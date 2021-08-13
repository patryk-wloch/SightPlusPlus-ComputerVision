// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <spdlog\spdlog.h>

#include "output_stream_controller.hpp"


OutputStreamController::OutputStreamController(const bool show_depth_window, const bool show_color_window)
	: show_depth_output_(show_depth_window), show_color_output_(show_color_window)
{
	SPDLOG_INFO("Setting up output stream windows");
	if (show_depth_window)
	{
		depth_output_window_ = "Depth Image";
		namedWindow(depth_output_window_, cv::WINDOW_AUTOSIZE);
	}
	if (show_color_window)
	{
		color_output_window_ = "Color Image";
		namedWindow(color_output_window_, cv::WINDOW_AUTOSIZE);
	}
}

//void OutputStreamController::stream_to_windows(const rs2::frame& depth_frame, cv::Mat depth_matrix, const rs2::video_frame& color_frame, cv::Mat color_matrix, const std::vector<ClassificationItem>& vector) const
//{
//	if (show_depth_output_)
//	{
//		output_to_depth_window(depth_frame);
//	}
//
//	if (show_color_output_)
//	{
//		output_to_color_window(color_matrix, depth_matrix, vector);
//	}
//}

//void OutputStreamController::output_to_depth_window(const rs2::frame& frame) const
//{
//	const auto width = frame.as<rs2::video_frame>().get_width();
//	const auto height = frame.as<rs2::video_frame>().get_height();
//
//	const cv::Mat depth_mat(cv::Size(width, height), CV_8UC3, const_cast<void*>(frame.get_data()), cv::Mat::AUTO_STEP);
//	imshow(depth_output_window_, depth_mat);
//}
//void OutputStreamController::output_to_color_window(cv::Mat color_matrix, cv::Mat depth_matrix, const std::vector<ClassificationItem>& vector) const
//{
//
//	//const auto width = frame.as<rs2::video_frame>().get_width();
//	//const auto height = frame.as<rs2::video_frame>().get_height();
//
//	//const cv::Mat color_mat(cv::Size(width, height), CV_8UC3, const_cast<void*>(frame.get_data()), cv::Mat::AUTO_STEP);
//
//
//	for (auto&& item : vector)
//	{
//		cv::Rect object(item.bottom_left.x, item.bottom_left.y, item.top_right.x - item.bottom_left.x, item.top_right.y - item.bottom_left.y);
//		object = object & cv::Rect(0, 0, depth_matrix.cols, depth_matrix.rows);
//
//		std::ostringstream ss;
//		ss << item.name << ", ID:";
//		ss << item.id << ", cond:";
//		ss << item.confidence << "\n msg: ";
//		ss << item.msg << "";
//		//ss << std::setprecision(2) << item.distance << " meters away";
//		cv::String conf(ss.str());
//
//		rectangle(color_matrix, object, cv::Scalar(0, 255, 0));
//		int base_line = 0;
//		cv::Size label_size = cv::getTextSize(ss.str(), cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &base_line);
//
//		auto center = (object.br() + object.tl()) * 0.5;
//		center.x = center.x - label_size.width / 2;
//
//		rectangle(
//			color_matrix,
//			cv::Rect(cv::Point(center.x, center.y - label_size.height),
//				cv::Size(label_size.width, label_size.height + base_line)),
//			cv::Scalar(255, 255, 255), cv::FILLED);
//
//		cv::putText(color_matrix, ss.str(), center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
//
//	}
//
//	cv::imshow(color_output_window_, color_matrix);
//}


bool OutputStreamController::should_receive_new_frames() const
{
	return wait_key() && is_depth_window_ready() && is_color_window_ready();
}

bool OutputStreamController::wait_key() const
{
	if (show_depth_output_ || show_color_output_)
	{
		return cv::waitKey(1) < 0;
	}
	return true;
}

bool OutputStreamController::is_depth_window_ready() const
{
	if (show_depth_output_)
	{
		return getWindowProperty(depth_output_window_, cv::WND_PROP_AUTOSIZE) >= 0;
	}
	return true;
}

bool OutputStreamController::is_color_window_ready() const
{
	if (show_color_output_)
	{
		return getWindowProperty(color_output_window_, cv::WND_PROP_AUTOSIZE) >= 0;
	}
	return true;
}
