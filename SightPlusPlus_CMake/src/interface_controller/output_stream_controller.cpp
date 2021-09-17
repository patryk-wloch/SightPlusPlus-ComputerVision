// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <spdlog\spdlog.h>

#include "output_stream_controller.hpp"


OutputStreamController::OutputStreamController(const bool show_depth_window, const bool show_color_window)
	: show_depth_output(show_depth_window), show_color_output(show_color_window)
{
	SPDLOG_INFO("Setting up output stream windows");
	if (show_depth_window)
	{
		depth_output_window = "Depth Image";
		namedWindow(depth_output_window, cv::WINDOW_AUTOSIZE);
	}
	if (show_color_window)
	{
		color_output_window = "Color Image";
		namedWindow(color_output_window, cv::WINDOW_AUTOSIZE);
	}
}




bool OutputStreamController::should_receive_new_frames() const
{
	return wait_key() && is_depth_window_ready() && is_color_window_ready();
}

bool OutputStreamController::wait_key() const
{
	if (show_depth_output || show_color_output)
	{
		return cv::waitKey(1) < 0;
	}
	return true;
}

bool OutputStreamController::is_depth_window_ready() const
{
	if (show_depth_output)
	{
		return getWindowProperty(depth_output_window, cv::WND_PROP_AUTOSIZE) >= 0;
	}
	return true;
}

bool OutputStreamController::is_color_window_ready() const
{
	if (show_color_output)
	{
		return getWindowProperty(color_output_window, cv::WND_PROP_AUTOSIZE) >= 0;
	}
	return true;
}

void OutputStreamController::stream(cv::Mat& color_matrix, cv::UMat& depth_matrix, std::vector<tracked_object>& objects) {
	if (this->show_color_output) {
		stream_color(color_matrix, objects);
	}
}


void OutputStreamController::stream_color(cv::Mat& color_matrix, std::vector<tracked_object>& objects) {

	for (auto& object : objects) {
		std::string text = cv::format("human, %f m, id %d", object.second.distance, object.second.id);
		cv::putText(color_matrix, text, cv::Point2f(object.second.bounding_box.x * (1.0 / COMP_SCALE), object.second.bounding_box.y * (1.0 / COMP_SCALE)), cv::FONT_HERSHEY_PLAIN, 2, object.second.color, 2);
		cv::Rect2d bounding_box(object.second.bounding_box.x * (1.0 / COMP_SCALE), object.second.bounding_box.y * (1.0 / COMP_SCALE), object.second.bounding_box.width * (1.0 / COMP_SCALE), object.second.bounding_box.height * (1.0 / COMP_SCALE));
		cv::rectangle(color_matrix, bounding_box, object.second.color, 2);
	}

	cv::imshow(color_output_window, color_matrix);
}

cv::Scalar OutputStreamController::get_random_color() {
	srand(time(0));

	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	return cv::Scalar(r, g, b, 255);
}