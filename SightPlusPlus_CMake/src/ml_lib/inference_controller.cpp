 // License: Apache 2.0. See LICENSE file in root directory.
 // Copyright(c) 2021 Sight++. All Rights Reserved.

// The OpenVINO interface in this file has been loosely inspired by some of the public OpenVINO code samples,
// e.g., https://github.com/IntelRealSense/librealsense/tree/master/wrappers/openvino
// or https://github.com/openvinotoolkit/openvino/blob/master/inference-engine/samples/hello_classification/main.cpp
// and https://github.com/openvinotoolkit/openvino/blob/master/inference-engine/samples/object_detection_sample_ssd/main.cpp
// but has been rewritten to account for integration with object tracking and depth measurements, support of 
// various types of neural networks, utilisation of, and safety with respect to, concurrency, and other crucial aspects. 
// As such, the final result bears little resemblance to any pre-existent publicly available code.


#include "inference_controller.hpp"

InferenceController::InferenceController(const std::string& path_to_model,const std::string& config) : path_to_model(path_to_model), config(config) {};

bool InferenceController::start() {
	try {
		prepare_queue();

		//Initialise OpenVINO Inference Engine
		InferenceEngine::Core core;

		//Option for deploying computation on both GPU and CPU
		if (config == "MULTI") {
			core.SetConfig({ {"MULTI_DEVICE_PRIORITIES","GPU,CPU"} }, "MULTI");
		}

		SPDLOG_INFO("Loaded Inference Engine Core");

		//Read layout of the CNN Network
		InferenceEngine::CNNNetwork network_main;
		network_main = core.ReadNetwork(path_to_model);

		//Get basic info about the network's input
		input_info = network_main.getInputsInfo();

		for (auto& item : input_info) {
			if (item.second->getInputData()->getTensorDesc().getDims().size() == 4) {
				input_layer = item.first;
				auto input_data = item.second;

				//Set input precision - unsigned 8-bit is chosen as it's compatible with all different architectures
				input_data->setPrecision(InferenceEngine::Precision::U8);

			}

			// Added suppport for hybrid networks with R-FCN/R-CNN input format
			else if (item.second->getInputData()->getTensorDesc().getDims().size() == 2) {
				input_info_layer = item.first;
				item.second->setPrecision(InferenceEngine::Precision::FP32);
			}
		}

		//Get basic info about the network's output
		InferenceEngine::OutputsDataMap output_info = network_main.getOutputsInfo();
		auto output_data = output_info.begin()->second;

		//Set output precision - 32-bit floating point is chosen as it's supported on all architectures
		output_data->setPrecision(InferenceEngine::Precision::FP32);
		output_layer = output_info.begin()->first;

		//Find a post-processing DetectionOutput layer if exists, from: 
		//https://github.com/openvinotoolkit/openvino/blob/master/inference-engine/samples/object_detection_sample_ssd/main.cpp
		if (auto ngraph_function = network_main.getFunction()) {
			for (const auto& out : output_info) {
				for (const auto& op : ngraph_function->get_ops()) {
					if (op->get_type_info() == ngraph::op::DetectionOutput::type_info && op->get_friendly_name() == out.second->getName()) {
						output_layer = out.first;
						output_data = out.second;
					}
				}
			}
		}
		//Create Executable Network instance and load the specified CNN Network

		exec_network_main = core.LoadNetwork(network_main, config);

		SPDLOG_INFO("Loaded network {} into Inference Engine", path_to_model);

		//Create test Inference Request instance;
		InferenceEngine::InferRequest infer_request = exec_network_main.CreateInferRequest();

		//Get information about the inference request input and output
		auto input_dimensions = infer_request.GetBlob(input_layer)->getTensorDesc().getDims();
		auto output_dimensions = infer_request.GetBlob(output_layer)->getTensorDesc().getDims();


		channels_count = input_dimensions.at(1);
		input_height = input_dimensions.at(2);
		input_width = input_dimensions.at(3);

		max_proposals = output_dimensions.at(2);
		object_size = output_dimensions.at(3);

		SPDLOG_INFO("Prepared test instance of Inference Request");
		return true;
	}
	catch (const std::exception& e) {
		SPDLOG_ERROR(e.what());
	}
}

void InferenceController::prepare_queue() {
	for (unsigned int i = 0; i < MAX_OBJECT_COUNT; i++) {
		free_ids.push(i);
	}
}

void InferenceController::process_frames(const cv::UMat& color_matrix, const cv::UMat& depth_matrix) {

	try {
		//Check if live objects overlap, if yes then lock them to prevent restarting tracking information
		for (auto& object1 : objects) {
			for (auto& object2 : objects) {
				if (calculate_overlap(object1.second, object2.second) > 0.2 && object1.second.id != object2.second.id) {
					object1.second.lock = true;
					object2.second.lock = true;
				}
			}
		}

		//Resize input matrix to match network specification
		cv::Mat img_input;
		cv::resize(color_matrix, img_input, cv::Size(input_height, input_width));
		size_t image_size = (size_t)input_height * (size_t)input_width;

		infer_request = exec_network_main.CreateInferRequest();

		const auto input_blob = infer_request.GetBlob(input_layer);
		input_buffer = InferenceEngine::as<InferenceEngine::MemoryBlob>(input_blob);

		//Prepare output BLOB and corresponding buffer space in memory
		const auto output_blob = infer_request.GetBlob(output_layer);
		output_buffer = InferenceEngine::as<InferenceEngine::MemoryBlob>(output_blob);

		//Get write access to input buffer
		auto input_access_holder = input_buffer->wmap();
		unsigned char* input_data = input_access_holder.as<unsigned char*>();

		//Fill input buffer with image data
		for (size_t pixel = 0; pixel < image_size; ++pixel)
		{
			for (size_t ch = 0; ch < channels_count; ++ch)
			{
				input_data[ch * image_size + pixel] = img_input.at<cv::Vec3b>(pixel)[ch];
			}
		}

		// Adaptation for R-CNN and R-FCN networks, inspired by
		// https://github.com/openvinotoolkit/openvino/blob/master/inference-engine/samples/object_detection_sample_ssd/main.cpp
		if (input_info_layer != "") {
			const auto input_info_blob = infer_request.GetBlob(input_info_layer);
			input_info_buffer = InferenceEngine::as<InferenceEngine::MemoryBlob>(input_info_blob);

			auto input_info_dimensions = input_info_blob->getTensorDesc().getDims().at(1);
			auto input_info_access_holder = input_info_buffer->wmap();
			float* data_info = input_info_access_holder.as<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>();

			data_info[input_info_dimensions + 0] = static_cast<float>(input_info[input_layer]->getTensorDesc().getDims()[2]);
			data_info[input_info_dimensions + 1] = static_cast<float>(input_info[input_layer]->getTensorDesc().getDims()[3]);

			for (unsigned int k = 2; k < input_info_dimensions; k++) {
				data_info[input_info_dimensions + k] = 1.0f;
			}


		}

		//Infer!
		infer_request.Infer();

		// Get read access to output buffer
		auto output_access_holder = output_buffer->rmap();
		const float* detections = output_access_holder.as<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>();

		tbb::concurrent_vector<DetectionResult> candidate_objects;

		SPDLOG_INFO("PROCESSING OUTPUTS FROM IE");

		// Process possible detections concurrently
		tbb::parallel_for(tbb::blocked_range<int>(0, max_proposals), [&](tbb::blocked_range<int> r) {

			for (unsigned int detection_index = r.begin(); detection_index < r.end(); detection_index++) {
				float image_id = detections[detection_index * object_size + 0];

				if (image_id < 0) break;

				int object_label = detections[detection_index * object_size + 1];
				float confidence = detections[detection_index * object_size + 2];
				int xmin = (int)(detections[detection_index * object_size + 3] * color_matrix.cols);
				int ymin = (int)(detections[detection_index * object_size + 4] * color_matrix.rows);
				int xmax = (int)(detections[detection_index * object_size + 5] * color_matrix.cols);
				int ymax = (int)(detections[detection_index * object_size + 6] * color_matrix.rows);

				// Currently set to only detect objects with label 1 - human, can be changed as required
				if (confidence > 0.5 && object_label == 1) {

					xmin = std::max(0, xmin);
					ymin = std::max(0, ymin);
					xmax = std::min(color_matrix.cols, xmax);
					ymax = std::min(color_matrix.rows, ymax);


					cv::Rect2d object(xmin, ymin, xmax - xmin, ymax - ymin);

					cv::UMat object_depth = depth_matrix(object);

					double distance = DEPTH ? get_distance(object_depth) : 0;

					DetectionResult current_detection = { object_label, distance, distance, 0, object };

					// Add provisional object to the candidate objects vector
					candidate_objects.push_back(current_detection);

				}
			}
			});
		//SPDLOG_INFO("PROCESSED OUTPUTS FROM IE");

		std::vector<tracked_object> new_objects;

		// Check all candidate objects against existent live objects to remove duplicates
		for (auto & candidate_object : candidate_objects) {

					bool check_tracking = false;

					for (auto& tracked_object : objects) {
						float overlap = calculate_overlap(tracked_object.second, candidate_object);

						if (overlap > 0.25) {
							check_tracking = true;

							// If overlap between 0.35 and 0.7 assume it's the same object, but restart tracker
							if (tracked_object.second.lock == false && overlap < 0.7) {
								tracked_object.first = ObjectTracker::create_tracker();
								tracked_object.first->init(color_matrix, candidate_object.bounding_box);

							}

							tracked_object.second.no_rc_counter = 0;
							break;

						}

					}

					// If no existent live object found, create a new object
					if (!check_tracking) {
						candidate_object.color = get_random_color();
						 
						free_ids.try_pop(candidate_object.id);
						auto tracker = ObjectTracker::create_tracker();
						tracker->init(color_matrix, candidate_object.bounding_box);
						candidate_object.first_bounding_box = candidate_object.bounding_box;
						new_objects.push_back(tracked_object(tracker, candidate_object));
					}



				}


		for (auto &object : new_objects) {
			objects.push_back(object);
		}
		//SPDLOG_INFO("PROCESSED CANDIDATE OBJECTS");

	}
	catch (const std::exception& e) {
		SPDLOG_ERROR(e.what());
		abort();
	}
}


float InferenceController::calculate_overlap(DetectionResult& object_tracked, DetectionResult& object_detected) {

	float intersection_area = (object_tracked.bounding_box & object_detected.bounding_box).area();

	float reference_area = object_tracked.bounding_box.area() + object_detected.bounding_box.area() - intersection_area;
	float overlap1 = ((object_tracked.bounding_box & object_detected.bounding_box) & object_tracked.bounding_box).area() / object_tracked.bounding_box.area();
	float overlap2 = ((object_tracked.bounding_box & object_detected.bounding_box) & object_detected.bounding_box).area() / object_detected.bounding_box.area();

	float overlap3 = intersection_area / reference_area;

	return std::max({ overlap1, overlap2, overlap3 });
}

cv::Scalar InferenceController::get_random_color() {
	srand(time(0));

	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	return cv::Scalar(r, g, b, 255);
}