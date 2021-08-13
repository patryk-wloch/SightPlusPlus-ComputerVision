// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include "spdlog/spdlog.h"
#include <inference_engine.hpp>
#include "classification_result.hpp"
#include "service_controller.hpp"
#include "setup_helper.hpp"
#include "interface_controller/api_controller.hpp"
#include "interface_controller/api_impl_websocket.cpp"
#include "interface_controller/output_stream_controller.hpp"
#include "ml_lib/inference_controller.hpp"

int main(int argc, char** argv)
{
#define _SOLUTIONDIR = R"($(SolutionDir))"

	rs2::pipeline pipe;
	rs2::config cfg;

	auto stream_depth = false;
	auto stream_color = false;
	auto port = 7979;
	auto theme = 0;
	std::set<std::string> outdoors_model = { "MobileNetSSD_deploy" };
	std::set<std::string> indoors_model = { "no_bn" };

	setup_logging();

	SPDLOG_INFO("Starting");

	/// <summary>
	/// This section is to capture the flags from the user. 
	/// Example flags are: 
	/// 1) realsense		: This is used run the system from the camera.
	/// 2) -rec hello.bag	: This is used to record the current input into a file with the name following the flag.
	/// 3) -play hello.bag  : This is used to play the file from the file following the flag.
	/// 4) -depth			: This is used to show the depth stream in a window
	/// 5) -color			: This is used to show the color stream in a window
	/// 6) -port			: This is used to select the port the websocket server runs on, default is 7979
	/// 7) -caffe no_bn		: This is used to import the caffe-based network named no_bn.caffemodel etc.
	/// 8) -yolo yolo		: This is used to import the darknet-based network (YoloV3).
	/// 9) -outdoors		: This is used to set up object detection networks, frame resolution and the prioritiser for outdoors environment.
	/// 10) -indoors		: This is used to set up object detection networks, frame resolution and the prioritiser for indoors environment.
	/// 11) -covid			: This is used to set up object detection networks, frame resolution and the prioritiser for Outdoors environment and uses a prioritiser that enforces social distancing.
	/// <summary>
	/// <param name="argc"></param>
	/// <param name="argv"></param>
	/// <returns></returns>
	if (argc > 1)
	{
		SPDLOG_INFO("Flags found");
		for (size_t i = 1; i < argc; i++)
		{
			// Capture next arg
			std::string next_arg = argv[i];

			SPDLOG_INFO("Next flag is {}", next_arg);
			if (next_arg.compare("realsense") == 0)
			{
				SPDLOG_INFO("Streaming from camera");
				cfg.enable_stream(RS2_STREAM_DEPTH, 848, 480, RS2_FORMAT_Z16, 15);
				cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_BGR8, 15);
				continue;
			}

			if (next_arg.compare("-rec") == 0 && (i + 1) < argc)
			{
				try
				{
					std::string file_ = argv[++i];
					std::string path_ = ".\\recordings\\" + file_;
					SPDLOG_INFO("Saving stream to file: {}", path_);
					cfg.enable_record_to_file(path_);
					continue;
				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with recording: {}", exception.what());
				}
			}
			else if (next_arg.compare("-rec") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing required flag for recording, record to what file?");
				continue;
			}

			if (next_arg.compare("-play") == 0 && (i + 1) < argc)
			{
				try
				{
					std::string file_ = argv[++i];
					std::string path_ = file_;
					SPDLOG_INFO("playing from file:  {}", path_);
					cfg.enable_device_from_file(path_);
					continue;
				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with playing from file: {}", exception.what());
				}
			}
			else if (next_arg.compare("-play") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing required flag for recording, play from what file?");
				continue;
			}

			if (next_arg.compare("-depth") == 0)
			{
				SPDLOG_INFO("Streaming depth output to window");
				stream_depth = true;
				continue;
			}

			if (next_arg.compare("-color") == 0)
			{
				SPDLOG_INFO("Streaming color output to window");
				stream_color = true;
				continue;
			}

			if (next_arg.compare("-port") == 0 && (i + 1) < argc)
			{
				port = std::atoi(argv[++i]);
				SPDLOG_INFO("Using port {} for websocket", std::to_string(port));
				continue;
			}
			else if (next_arg.compare("-port") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing value for flag -port");
				continue;
			}

			if (next_arg.compare("-caffe") == 0 && (i + 1) < argc)
			{
				try
				{

				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with loading caffe-based network from file: {}", exception.what());
				}
			}
			else if (next_arg.compare("-caffe") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing flag/argument for loading the caffe-based network");
				continue;
			}

			if (next_arg.compare("-yolo") == 0 && (i + 1) < argc)
			{
				try
				{

				}
				catch (const std::exception& exception)
				{
					SPDLOG_CRITICAL("Error with loading darknet-based yolo network from file: {}", exception.what());
				}
			}
			else if (next_arg.compare("-yolo") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing flag/argument for loading the darknet-based yolo network");
				continue;
			}

		
		}
	}



	
	else
	{
		SPDLOG_INFO("Using default recording");
		cfg.enable_device_from_file(".\\recordings\\outdoors.bag");
	}

	auto config = pipe.start(cfg);

	SPDLOG_INFO("Streaming profiles:");
	for (auto&& stream_profile : pipe.get_active_profile().get_streams())
	{
		auto profile = stream_profile.as<rs2::video_stream_profile>();
		SPDLOG_INFO("{}: {}x{}", profile.stream_name(), profile.width(), profile.height());
	}

	InferenceController inference_controller;
	if (!inference_controller.start()) return EXIT_FAILURE;
	// TODO Read command line parameters for which models to use?
	// TODO Add correct paths for testing
	// TODO Add command line parameter for files to use?

	auto profile = config.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();


	// Todo: load prio model from flag

	SPDLOG_INFO("Setting up output API and API users");
	ApiController api;
	//ApiWebSocketImpl websocket_api_user(port, Priority::HIGH);
	//api.add_user(websocket_api_user);

	SPDLOG_INFO("Setting up Output Stream Windows");
	OutputStreamController output_stream_controller(stream_depth, stream_color);

	ServiceController service(pipe, inference_controller, api, output_stream_controller, profile);
	service.main();
}
