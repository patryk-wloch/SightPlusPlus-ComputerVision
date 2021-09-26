// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include <vector>

#include "../helpers.hpp"
#include "tbb/concurrent_vector.h"
#include "tbb/parallel_for_each.h"
#include "spdlog/spdlog.h"
#include <inference_engine.hpp>
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_queue.h>
#include <ngraph/ngraph.hpp>
#include <opencv2/tracking/tracker.hpp>
#include "../config.hpp"
#include "object_tracking.hpp"

// Class controlling all object detection and recognition actions

class InferenceController {


	std::string path_to_model;
	std::string config;
	unsigned int channels_count;
	unsigned int input_height;
	unsigned int input_width;

	unsigned int max_proposals;
	unsigned int object_size;

	InferenceEngine::MemoryBlob::Ptr input_buffer;
	InferenceEngine::MemoryBlob::Ptr input_info_buffer;
	InferenceEngine::MemoryBlob::Ptr output_buffer;

	InferenceEngine::InferRequest infer_request;
	InferenceEngine::ExecutableNetwork exec_network_main;

	InferenceEngine::InputsDataMap input_info;
	std::string input_layer;
	std::string input_info_layer;
	std::string output_layer;

public:

	std::vector<tracked_object> objects;
	tbb::concurrent_queue<int> free_ids;

	InferenceController(const std::string& path_to_model, const std::string& config);

	bool start();
	void prepare_queue();

	void process_frames(const cv::UMat& color_matrix, const cv::UMat& depth_matrix);

	float calculate_overlap(DetectionResult& object_tracked, DetectionResult& object_detected);

	cv::Scalar get_random_color();
	};

