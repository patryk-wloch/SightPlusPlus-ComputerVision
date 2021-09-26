// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include "spdlog/spdlog.h"
#include <inference_engine.hpp>

#include "service_controller.hpp"
#include "helpers.hpp"
#include "interface_controller/api_controller.hpp"
#include "interface_controller/api_impl_websocket.cpp"
#include "interface_controller/output_stream_controller.hpp"
#include "ml_lib/inference_controller.hpp"
#include "config.hpp"

// Most of the code in this file is taken from Gen 1 Sight++, apart from changes to initiate OpenVINO API at startup

int main(int argc, char** argv)
{
#define _SOLUTIONDIR = R"($(SolutionDir))"

	rs2::pipeline pipe;
	rs2::config cfg;

	auto stream_depth = false;
	auto stream_color = false;
	auto port = 7979;
	auto theme = 0;
	std::string path_to_model = "models/ssd_mobilenet_v2_coco.xml";
	std::string vino_config = "GPU";
	std::string tracker = "MOSSE";

	setup_logging();

	SPDLOG_INFO("Starting");

	if (argc > 1)
	{
		SPDLOG_INFO("Flags found");

		for (size_t i = 1; i < argc; i++)
		{
			// Capture next arg
			std::string next_arg = argv[i];

			SPDLOG_INFO("Next flag is {}", next_arg);
			if (next_arg.compare("-realsense") == 0)
			{
				SPDLOG_INFO("Streaming from camera");
				cfg.enable_stream(RS2_STREAM_DEPTH, 848, 480, RS2_FORMAT_Z16, CAMERA_FPS);
				cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_BGR8, CAMERA_FPS);
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
				path_to_model = argv[++i];
			}

			else if (next_arg.compare("-caffe") == 0 && !((i + 1) < argc))
			{
				SPDLOG_ERROR("Missing argument for loading the network");
				continue;
			}

			if (next_arg.compare("-vino") == 0 && (i + 1) < argc)
			{
				vino_config = argv[++i];
			}
			else if (next_arg.compare("-vino") == 0 && !((i + 1) < argc)) 
			{
				SPDLOG_ERROR("Missing argument for the -vino flag");
				continue;
			}

			if (next_arg.compare("-KCF") == 0) {
				tracker = "KCF";
			}
		
		}
	}
	
	else
	{
		SPDLOG_ERROR("Missing required runtime flags");
		return EXIT_FAILURE;
	}

	auto config = pipe.start(cfg);

	SPDLOG_INFO("Streaming profiles:");
	for (auto&& stream_profile : pipe.get_active_profile().get_streams())
	{
		auto profile = stream_profile.as<rs2::video_stream_profile>();
		SPDLOG_INFO("{}: {}x{}", profile.stream_name(), profile.width(), profile.height());
	}

	InferenceController inference_controller(path_to_model, vino_config);
	if (!inference_controller.start()) return EXIT_FAILURE;


	auto profile = config.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
	auto intr = profile.get_intrinsics();
	ObjectTracker object_tracker(inference_controller.free_ids, intr, tracker);

	SPDLOG_INFO("Setting up output API and API users");
	ApiController api;

	//ApiWebSocketImpl websocket_api_user(port, Priority::HIGH);
	//api.add_user(websocket_api_user);

	
	OutputStreamController output_stream_controller(stream_depth, stream_color);
	SPDLOG_INFO("Setting up Output Stream Windows");

	ServiceController service(pipe, inference_controller, api, output_stream_controller, profile, object_tracker);
	service.main();
}
