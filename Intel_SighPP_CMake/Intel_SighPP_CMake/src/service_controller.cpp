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
struct detectionResults
{
	float xmin = 0.0;      // coordinate of bounding box
	float ymin = 0.0;      // coordinate of bounding box
	float xmax = 0.0;      // coordinate of bounding box
	float ymax = 0.0;      // coordinate of bounding box
	int label = 0;
	double distance = 0;

};
ServiceController::ServiceController(
	rs2::pipeline& pipe, InferenceController& inference_controller, ApiController& api_controller, OutputStreamController output_controller, const rs2::video_stream_profile& profile)
	: pipe_(pipe), inference_controller_(inference_controller), api_controller_(api_controller), output_stream_controller_(std::move(output_controller)), profile_(profile)
{}

int ServiceController::main() try {
	int process_frame_counter = 0;
	int translate_frame_counter = 0;
	int infer_frame_counter = 0;
	SPDLOG_INFO("Instantiated Service Controller");
	std::mutex lock;
	std::queue< std::pair<std::unique_ptr<cv::Mat>, std::unique_ptr<cv::Mat>>> mats;
	int counter = 0;
	rs2::decimation_filter dec_filter;
	rs2::spatial_filter spat_filter;
	rs2::hole_filling_filter  hole_filter;
	dec_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, 2);
	spat_filter.set_option(RS2_OPTION_HOLES_FILL, 3);
	hole_filter.set_option(RS2_OPTION_HOLES_FILL, 1);
	rs2::align align_to(RS2_STREAM_COLOR);
	rs2::colorizer color_map;
	rs2_intrinsics intr = profile_.get_intrinsics();
	crop = create_rect(profile_);

	rs2::frame_queue processed_frames;

	//Frame processing thread
	std::thread video_thread([&]() {
		
		while (output_stream_controller_.should_receive_new_frames()) {
			rs2::frameset data;

			if (pipe_.poll_for_frames(&data)) {

				//Decimation filter
				rs2::frameset processed = dec_filter.process(data);

				//Spatial filter
				processed = spat_filter.process(processed);

				//Hole-filling filter
				processed = hole_filter.process(processed);

				//Align color and depth frame to each other
				rs2::frameset aligned = align_to.process(processed);

				processed_frames.enqueue(aligned);
				process_frame_counter++;
				SPDLOG_INFO("Enqueued a frame {}", process_frame_counter);
			}
		}
		});

	rs2::frameset curr_frame;



	std::thread cv_translation_thread([&]() {
		while (output_stream_controller_.should_receive_new_frames()) {

			if (processed_frames.poll_for_frame(&curr_frame)) {
				if (mats.size() > 3) {
					SPDLOG_INFO("SKIPPED"); continue;
				}
					auto color_matrix = frame_to_mat(curr_frame.get_color_frame());
					auto depth_matrix = depth_frame_to_meters(curr_frame.get_depth_frame());
					color_matrix = color_matrix(crop);
					depth_matrix = depth_matrix(crop);
					mats.push(std::pair<std::unique_ptr<cv::Mat>, std::unique_ptr<cv::Mat>> { std::make_unique<cv::Mat>(color_matrix), std::make_unique<cv::Mat>(depth_matrix)});
					translate_frame_counter++;
					SPDLOG_INFO("Translated a frame {}", translate_frame_counter);
		
			}

		}
		});

	bool skipper = false;
	auto& results = inference_controller_.results_;
	while (output_stream_controller_.should_receive_new_frames()) {
		
		try {
			if (mats.empty()) continue;
			if ((*(mats.front().first)).total() == 960*720)  {
		

				cv::Mat curr_color_matrix = *(mats.front().first);

				cv::Mat curr_depth_matrix = *(mats.front().second);


					SPDLOG_INFO("Conversion and cropping complete");


					if (!skipper) {
						inference_controller_.process_frames(curr_color_matrix, curr_depth_matrix);
					}



					for (unsigned int objectIndex = 0; objectIndex < results.size(); objectIndex++)
					{
						// Display label and distance text on the bounding box
						std::string textToDisplay = cv::format("%d, %f m", results.at(objectIndex).label, results.at(objectIndex).distance);
						cv::putText(curr_color_matrix, textToDisplay, cv::Point2f(results.at(objectIndex).xmin, results.at(objectIndex).ymin), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0, 255), 2);
						// Draw bounding box
						cv::rectangle(curr_color_matrix, cv::Point2f(results.at(objectIndex).xmin, results.at(objectIndex).ymin), cv::Point2f(results.at(objectIndex).xmax, results.at(objectIndex).ymax), cv::Scalar(0, 255, 0, 255), 1);
					}

					infer_frame_counter++;
					SPDLOG_INFO("Inferred a frame {}", infer_frame_counter);

					// Show the image in the window
					//cv::imshow("DEPTH", depth_matrix);
			/*		if (counter == 100) {
						return 0;
					}*/
					cv::imshow("realsense", curr_color_matrix);
					cv::waitKey(1);
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
	if (profile.width() / static_cast<float>(profile.height()) > WHRatio)
	{
		crop_size = cv::Size(static_cast<int>(profile.height() * WHRatio), profile.height());
	}
	else
	{
		crop_size = cv::Size(profile.width(), static_cast<int>(profile.width() / WHRatio));
	}

	return cv::Rect(cv::Point((profile.width() - crop_size.width) / 2, (profile.height() - crop_size.height) / 2), crop_size);
}
