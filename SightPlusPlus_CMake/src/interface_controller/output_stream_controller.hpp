// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once
#include "librealsense2/rs.hpp"
#include "opencv2/opencv.hpp"
#include "../config.hpp"

class OutputStreamController {

	std::string depth_output_window;
	bool show_depth_output;

	std::string color_output_window;
	bool show_color_output;

	rs2::colorizer color_map_;

public:


	OutputStreamController(const bool show_depth_window, const bool show_color_window);

	bool should_receive_new_frames() const;

	bool wait_key() const;
	bool is_depth_window_ready() const;
	bool is_color_window_ready() const;

	void stream(cv::Mat& color_matrix, cv::UMat& depth_matrix, std::vector<tracked_object>& objects);

	void stream_color(cv::Mat& color_matrix, std::vector<tracked_object>& objects);
	void stream_depth(cv::Mat& depth_matrix, std::vector<tracked_object>& objects);

	cv::Scalar get_random_color();

};