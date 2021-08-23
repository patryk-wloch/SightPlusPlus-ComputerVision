// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include "librealsense2/rs.hpp"
#include "librealsense2/rsutil.h"

#include "interface_controller/api_controller.hpp"
#include "interface_controller/output_stream_controller.hpp"
#include "ml_lib/inference_controller.hpp"
#include "ml_lib/object_tracking.hpp"

class ServiceController {

	rs2::pipeline pipe_;
	InferenceController inference_controller_;
	ApiController api_controller_;
	OutputStreamController output_stream_controller_;
	rs2::video_stream_profile profile_;
	ObjectTracker object_tracker;
	cv::Rect crop;

	const size_t inWidth = 640;
	const size_t inHeight = 480;
	const float WHRatio = inWidth / (float)inHeight;
	const float inScaleFactor = 0.007843f;
	const float meanVal = 127.5;

	int process_frame_counter = 0;
	int translate_frame_counter = 0;
	int infer_frame_counter = 0;

	std::queue< std::tuple<std::unique_ptr<cv::Mat>, std::unique_ptr<cv::UMat>, std::unique_ptr<cv::UMat>>> mats;

	rs2::decimation_filter dec_filter;
	rs2::spatial_filter spat_filter;
	rs2::hole_filling_filter  hole_filter;

	rs2::colorizer color_map;
	rs2_intrinsics intr = profile_.get_intrinsics();

	rs2::frame_queue processed_frames;

	bool skipper = false;


	std::vector<std::pair<std::string, long>> events;


public:

	ServiceController(
		rs2::pipeline& pipe, InferenceController& ml_controller, ApiController& api_controller, 
		OutputStreamController output_controller, const rs2::video_stream_profile& profile, ObjectTracker object_tracker);

	/// <summary>
	/// Main program loop. Receives and processes frames.
	/// </summary>
	/// <returns>Program exit code</returns>
	int main();

	/// <summary>
	/// Create a correctly sized cropped rectangle.
	/// Based on rs-dnn example.
	/// </summary>
	/// <param name="profile">Stream Profile containing width and height properties.</param>
	/// <returns></returns>
	cv::Rect create_rect(const rs2::video_stream_profile& profile) const;
};
