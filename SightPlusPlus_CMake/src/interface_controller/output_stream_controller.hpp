// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once
#include "librealsense2/rs.hpp"
#include "opencv2/opencv.hpp"


class OutputStreamController {

	std::string depth_output_window_;
	bool show_depth_output_;

	std::string color_output_window_;
	bool show_color_output_;

	rs2::colorizer color_map_;

public:

	virtual ~OutputStreamController() {};

	OutputStreamController(const bool show_depth_window, const bool show_color_window);

	/// <summary>
	/// Sends depth and color frame to show in output stream windows, if enabled.
	/// </summary>
	/// <param name="depth_frame">Realsense Depth Frame</param>
	/// <param name="depth_matrix">OpenCV matrix of the depth frame</param>
	/// <param name="color_frame">Realsense Color Frame</param>
	/// <param name="color_matrix">OpenCV matrix of the color frame</param>
	/// <param name="vector">Vector of recognised objects</param>
	//virtual void stream_to_windows(
	//	const rs2::frame& depth_frame, cv::Mat depth_matrix,
	//	const rs2::video_frame& color_frame, cv::Mat color_matrix,
	//	const std::vector<ClassificationItem>& vector) const;

	/// <summary>
	/// Output depth stream to window. Color map is pre-applied.
	/// </summary>
	/// <param name="frame">Depth frame with color-map pre-applied.</param>
	//virtual void output_to_depth_window(const rs2::frame& frame) const;

	/// <summary>
	/// Output color stream to window.
	/// Also draws rectangles around recognised objects.
	/// </summary>
	/// <param name="color_matrix">Color frame</param>
	/// <param name="depth_matrix">Depth frame</param>
	/// <param name="vector">Rectangles will be drawn around these items</param>
	//virtual void output_to_color_window(cv::Mat color_matrix, cv::Mat depth_matrix, const std::vector<ClassificationItem>& vector) const;

	/// <summary>
	/// Tells the system if the OpenCV stream output windows are ready, if enabled.
	/// In the case where no output stream windows are enabled, returns true.
	/// Based on the rs-dnn and rs-imshow examples.
	/// </summary>
	/// <returns>
	///   Returns TRUE if the windows are ready and can receive frames, or if no output stream windows are enabled.
	///   Returns FALSE if the windows can not receive frames.
	/// </returns>
	virtual bool should_receive_new_frames() const;

	virtual bool wait_key() const;
	virtual bool is_depth_window_ready() const;
	virtual bool is_color_window_ready() const;

};