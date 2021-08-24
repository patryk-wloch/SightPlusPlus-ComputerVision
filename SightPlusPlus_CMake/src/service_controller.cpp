// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <iostream>
#include <utility>
#include <algorithm>
#include <Windows.h>
#include "spdlog/spdlog.h"
#include <inference_engine.hpp>
#include "service_controller.hpp"
#include <opencv2/core/ocl.hpp>
#include "tbb/concurrent_vector.h"

#include <stdint.h>

ServiceController::ServiceController(
	rs2::pipeline& pipe, InferenceController& inference_controller, ApiController& api_controller, OutputStreamController output_controller, const rs2::video_stream_profile& profile, ObjectTracker object_tracker)
	: pipe(pipe), inference_controller(inference_controller), api_controller(api_controller), output_stream_controller(std::move(output_controller)), profile(profile), object_tracker(object_tracker)
{}

int ServiceController::main() try {

	SPDLOG_INFO("Instantiated Service Controller");

	//Setting different RealSense depth filters
	dec_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, DEC_FILTER);
	spat_filter.set_option(RS2_OPTION_HOLES_FILL, SPAT_FILTER);
	hole_filter.set_option(RS2_OPTION_HOLES_FILL, HOLE_FILTER);

	rs2::align align_to(RS2_STREAM_COLOR);

	crop = create_rect(profile);

	//Camera interface and frame processing thread
	std::thread video_thread([&]() {
		
		while (output_stream_controller.should_receive_new_frames()) {
			rs2::frameset data;

			if (pipe.poll_for_frames(&data)) {

				//Decimation filter
				rs2::frameset processed = dec_filter.process(data);

				//Spatial filter
				processed = spat_filter.process(processed);

				//Hole-filling filter
				processed = hole_filter.process(processed);

				//Align color and depth frame to each other
				rs2::frameset aligned = align_to.process(processed);

				processed_frames.enqueue(aligned);
	
			}
		}
		});

	rs2::frameset curr_frame;

	//RealSense Frame -> OpenCV Mat conversion thread
	std::thread cv_translation_thread([&]() {
		while (output_stream_controller.should_receive_new_frames()) {

			//Check if new frame available from camera / recording
			if (processed_frames.poll_for_frame(&curr_frame)) {

				//Skip the frame if too many in the queue for object detection
				if (mats.size() > 3) {
					continue;
				}

					//Convert the realsense frame to OpenCV matrices
					auto color_matrix = frame_to_mat(curr_frame.get_color_frame());
					auto depth_matrix = depth_frame_to_meters(curr_frame.get_depth_frame());

					//Crop the matrices to the same size
					color_matrix = color_matrix(crop);
					depth_matrix = depth_matrix(crop);

					//Compress the matrices
					cv::UMat color_matrix_comp;
					cv::UMat depth_matrix_comp;
					cv::resize(color_matrix, color_matrix_comp, cv::Size(), COMP_SCALE, COMP_SCALE);
					cv::resize(depth_matrix, depth_matrix_comp, cv::Size(), COMP_SCALE, COMP_SCALE);

					//Add converted matrices to the queue for object detection
					mats.push(std::tuple<std::unique_ptr<cv::Mat>, std::unique_ptr<cv::UMat>, std::unique_ptr<cv::UMat>> 
					{ std::make_unique<cv::Mat>(color_matrix), std::make_unique<cv::UMat>(color_matrix_comp), std::make_unique<cv::UMat>(depth_matrix_comp)});
		
			}

		}
		});

	//Main thread
	while (output_stream_controller.should_receive_new_frames()) {
		
		try {
			//If no matrices ready, skip
			if (mats.empty()) continue;
		
			//Check if the matrix in front of the queue fully written
			if ((*(std::get<0>(mats.front()))).total() == crop.area())  {

				//Allow camera time to adjust exposition before starting to process - 15 frames
				if (infer_frame_counter < 15) {
					mats.pop();
					infer_frame_counter++;
					continue;
				}

				//Retrieve current matrices from pointers
				cv::Mat color_matrix = *(std::get<0>(mats.front()));
				cv::UMat color_matrix_comp = *(std::get<1>(mats.front()));
				cv::UMat depth_matrix_comp = *(std::get<2>(mats.front()));

				//Object tracking is updated every frame
				object_tracker.update_all_trackers(objects, color_matrix_comp);
		
				//Object recognition is performed every second frame
				if(!skipper) inference_controller.process_frames(color_matrix_comp, depth_matrix_comp);

				//Send the results to output stream controller for displaying
				output_stream_controller.stream(color_matrix, depth_matrix_comp, objects);

				SPDLOG_INFO("Inferred a frame {}", ++infer_frame_counter);

				skipper = !skipper;
				mats.pop();

			}
		}
		catch (const std::exception& e) {
			SPDLOG_ERROR(e.what());
		}
	}
		return EXIT_SUCCESS;
	}


catch (const rs2::error& e)
{
	SPDLOG_ERROR("Realsense error, calling {} ({}): {}", e.get_failed_function(), e.get_failed_args(), e.what());
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	SPDLOG_ERROR(e.what());
	return EXIT_FAILURE;
}

cv::Rect ServiceController::create_rect(const rs2::video_stream_profile& profile) const
{
	cv::Size crop_size;
	if (profile.width() / static_cast<float>(profile.height()) > WH_RATIO)
	{
		crop_size = cv::Size(static_cast<int>(profile.height() * WH_RATIO), profile.height());
	}
	else
	{
		crop_size = cv::Size(profile.width(), static_cast<int>(profile.width() / WH_RATIO));
	}

	return cv::Rect(cv::Point((profile.width() - crop_size.width) / 2, (profile.height() - crop_size.height) / 2), crop_size);
}
