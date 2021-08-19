// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include <vector>

#include "../classification_result.hpp"
#include "ml_interface.hpp"
#include "../../cv-helpers.hpp"
#include "tbb/concurrent_vector.h"
#include "tbb/parallel_for_each.h"
#include "ml_interface.hpp"
#include "spdlog/spdlog.h"
#include "../setup_helper.hpp"
#include <inference_engine.hpp>
#include <tbb/concurrent_vector.h>
#include <ngraph/ngraph.hpp>
#include <opencv2/tracking/tracker.hpp>

/// <summary>
/// This class is designed as the object to control all object recognition networks and define corresponding functions 
/// </summary>
/// 
/// 
/// 
/// 
struct DetectionResult
{
int label = 0;
double distance = 0;
int id = 0;
cv::Rect2d bounding_box;
};

class InferenceController {


	std::string path_to_model_ = "models/ssd_mobilenet_v2_coco.xml";
	std::string config_ = "MULTI";
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
	InferenceController() { };
	InferenceController(std::string& path_to_model, std::string& config) {

		path_to_model_ = path_to_model;
		config_ = config;


	}
	std::vector<std::pair<cv::Ptr<cv::Tracker>, DetectionResult>> results_;
	std::queue<int> free_ids;
	bool start() {
		try {
			//Initialise OpenVINO Inference Engine
			InferenceEngine::Core core;
			free_ids.push(1);
			free_ids.push(2);
			free_ids.push(3);
			free_ids.push(4);
			free_ids.push(5);
			free_ids.push(6);
			free_ids.push(7);
			free_ids.push(8);
			free_ids.push(9);
			free_ids.push(10);
			free_ids.push(11);
			free_ids.push(12);
			free_ids.push(13);
			free_ids.push(14);
			free_ids.push(15);
			free_ids.push(16);
			free_ids.push(17);
			free_ids.push(18);
			free_ids.push(19);
			free_ids.push(20);
			free_ids.push(21);
			free_ids.push(22);
			free_ids.push(23);
			free_ids.push(24);
			free_ids.push(25); 
			free_ids.push(26);
			free_ids.push(27);
			free_ids.push(28);
			free_ids.push(29);

			if (config_ == "MULTI") {
				core.SetConfig({ {"MULTI_DEVICE_PRIORITIES","GPU,CPU"} }, "MULTI");
			}
			SPDLOG_INFO("Loaded Inference Engine Core");
			
			//Read layout of the CNN Network
			InferenceEngine::CNNNetwork network_main;
			network_main = core.ReadNetwork(path_to_model_);

			//Get basic info about the network's input
			input_info = network_main.getInputsInfo();

			for (auto& item : input_info) {
				if (item.second->getInputData()->getTensorDesc().getDims().size() == 4) {
					input_layer = item.first;
					auto input_data = item.second;

					//Set input precision - unsigned 8-bit is chosen as it's compatible with all different architectures
					input_data->setPrecision(InferenceEngine::Precision::U8);

				}
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
		
			exec_network_main = core.LoadNetwork(network_main, config_);

			SPDLOG_INFO("Loaded network into Inference Engine");

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

			SPDLOG_INFO("{}, {}, {}, {}, {}",channels_count, input_height, input_width, max_proposals, object_size);

			//Prepare input BLOB and corresponding buffer space in memory
		

			SPDLOG_INFO("Prepared instance of Inference Request");
			return true;
		}
		catch (const std::exception& e) {
			SPDLOG_ERROR(e.what());
		}
	}

	void process_frames(const cv::UMat& color_matrix, const cv::UMat& depth_matrix) {

		try {
			//Clear detections vector

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

			auto input_access_holder = input_buffer->wmap();
			unsigned char* input_data = input_access_holder.as<unsigned char*>();

			for (size_t pixel = 0; pixel < image_size; ++pixel)
			{
				for (size_t ch = 0; ch < channels_count; ++ch)
				{
					input_data[ch * image_size + pixel] = img_input.at<cv::Vec3b>(pixel)[ch];
				}
			}


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


			infer_request.Infer();


			auto output_access_holder = output_buffer->rmap();
			const float* detections = output_access_holder.as<InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type*>();



			std::vector<std::pair<cv::Ptr<cv::Tracker>, DetectionResult>> new_results;

			for (unsigned int detection_index = 0; detection_index < max_proposals; detection_index++) {
				float image_id = detections[detection_index * object_size + 0];

				if (image_id < 0) break;

				int object_label = detections[detection_index * object_size + 1];
				float confidence = detections[detection_index * object_size + 2];
				int xmin = (int)(detections[detection_index * object_size + 3] * color_matrix.cols);
				int ymin = (int)(detections[detection_index * object_size + 4] * color_matrix.rows);
				int xmax = (int)(detections[detection_index * object_size + 5] * color_matrix.cols);
				int ymax = (int)(detections[detection_index * object_size + 6] * color_matrix.rows);

				if (confidence > 0.5 && object_label == 1) {


					xmin = std::max(0, xmin);

					ymin = std::max(0, ymin);
					xmax = std::min(color_matrix.rows, xmax);
					ymax = std::min(color_matrix.cols, ymax);

					cv::Rect2d object(xmin, ymin, xmax - xmin, ymax - ymin);
		
					SPDLOG_INFO("Detected object, x {}, y {}, height {}, width {}", object.x, object.y, object.height, object.width);
					cv::UMat object_depth = depth_matrix(object);

					double distance = get_distance(object_depth);
						
					DetectionResult current_detection = {object_label, distance, 0, object };



					bool check_tracking = false;

					for (auto &tracked_object : results_) {
						SPDLOG_INFO("Checking new object against object x {}, y {}, height {}, width {}", tracked_object.second.bounding_box.x, tracked_object.second.bounding_box.y, tracked_object.second.bounding_box.height, tracked_object.second.bounding_box.width );
						float overlap = calculate_overlap(tracked_object.second, current_detection);
						if (overlap > 0.5) {
							check_tracking = true;
							tracked_object.second.distance = distance;
							break;

						}
					}

					if (!check_tracking) {
						current_detection.id = free_ids.front();
						free_ids.pop();
						auto tracker = create_tracker();
						tracker->init(color_matrix, object);
						new_results.push_back(std::pair<cv::Ptr<cv::Tracker>, DetectionResult> (tracker, current_detection));
					}
					


				}

			}
			for (auto result : new_results) {
				results_.push_back(result);
			}

		}
		catch (const std::exception& e) {
			SPDLOG_ERROR(e.what());
			abort();
		}
	}
		double get_distance(cv::UMat & object) {
			if (!object.isContinuous()) object = object.clone();
			object = object.reshape(0, 1);
			cv::Mat label, centers;
			cv::kmeans(object, 2, label, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 50, 0.1), 1, cv::KMEANS_RANDOM_CENTERS, centers);
			double distance = centers.at<float>(0) > centers.at<float>(1) ? centers.at<float>(1) : centers.at<float>(0);
			return distance;

	}

		cv::Ptr<cv::Tracker> create_tracker() {
			return cv::TrackerMOSSE::create();
		}

		float calculate_overlap(DetectionResult& object_tracked, DetectionResult& object_detected) {

			float intersection_area = (object_tracked.bounding_box & object_detected.bounding_box).area();

			float reference_area = object_tracked.bounding_box.area() + object_detected.bounding_box.area() - intersection_area;

	
			float overlap = intersection_area / reference_area;

			return overlap;
		}
	};

	/// <summary>
	/// Get all objects with their classes, distance and corresponding realtive locations within the frame
	/// and clears them afterwards, so that they are not used again to avoid reusing the same result in the following frames
	/// </summary>
	//std::vector<ClassificationResult> get_and_clear_results()
	//{
	//	std::vector<ClassificationResult> to_send = { results_.begin(), results_.end() };

	//	// Note that the clear function below is not concurrency-safe
	//	results_.clear();
	//	results_.shrink_to_fit();

	//	SPDLOG_INFO("Got and cleared {} classification results", to_send.size());

	//	return to_send;
	//}
	/// <summary>
	/// Receives new frames and pushes assign these frames to object recognition models for object dectection
	/// </summary>
	/// <param name="color_matrix">color matrix obtained from the camera</param>
 	/// <param name="depth_matrix">depth matrix obtained from the camera</param>



	/// <summary>
	/// Gets the size of the list containing all object recognition models added so far
	/// </summary>
	/// <returns>the number of object recognition models added so far</returns>


//SPDLOG_INFO("STARTING KMEANS");

			////cv::imshow("depth_matrix", object_depth);
			////std::string empty = "";
			////std::cin >> empty;
			//object_depth.convertTo(object_depth, CV_32F);
			///*			if (!object_depth.isContinuous()) {
			//				object_depth = object_depth.clone();
			//			}*/
			//cv::Mat color_mask(object_depth.rows, object_depth.cols, CV_8UC3, cv::Scalar(255, 0, 255));
			//object_depth = object_depth.reshape(0, 1);

			//cv::kmeans(object_depth, 2, label, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 0.01), 1, cv::KMEANS_RANDOM_CENTERS, centers);
			//cv::Mat mask = label.reshape(0, depth_matrix(object).rows);
			//mask.convertTo(mask, CV_8U);
			////cv::Mat distances = centers * depth_frame.get_units();
			////SPDLOG_INFO("{}, {}", distances.rows, distances.cols);


			//uchar center_object_pixel = mask.at<uchar>(mask.rows / 2, mask.cols / 2);
			//if (center_object_pixel == 0) {
			//	for (int i = 0; i < mask.rows; i++) {
			//		for (int j = 0; j < mask.cols; j++) {
			//			if (mask.at<uchar>(i, j) == 1) {
			//				mask.at<uchar>(i, j) = 0;
			//			}
			//			else {
			//				mask.at<uchar>(i, j) = 1;
			//			}
			//		}
			//	}
			//}



			///*	double minvalue;
			//	double maxvalue;

			//	cv::minMaxLoc(mask, &minvalue, &maxvalue);*/
			//	/*			SPDLOG_INFO("{}, {}", minvalue, maxvalue);*/

			//				//cv::imshow("mask",mask); 
			//color_mask.copyTo(color_matrix(object), mask);
			//SPDLOG_INFO("FINISHED KMEANS");


	/*	ml_controller_.new_frames(color_matrix, depth_matrix);

				SPDLOG_INFO("Prioritising results");
				auto prioritised_results = prioritiser_.prioritise(ml_controller_.get_and_clear_results());
				SPDLOG_INFO("Prioritised {} results", prioritised_results.size());
				for (int i = 0; i < prioritised_results.size(); i++) {
					SPDLOG_INFO(prioritised_results[i].to_string());
					SPDLOG_INFO("top_left: {} {}, right_bottom: {} {}", prioritised_results[i].top_left.x, prioritised_results[i].top_left.y, prioritised_results[i].bottom_right.x, prioritised_results[i].bottom_right.y);

				};*/
				/*float upixel[2];
				float vpixel[2];
				float upoint[3];
				float vpoint[3];

				std::vector<std::vector<float>> distance_matrix;
				if (prioritised_results.size() > 1) {
					for (int i = 0; i < prioritised_results.size(); i++) {
						for (int j = 0; j < prioritised_results.size(); j++)
						{
							auto object1 = prioritised_results[i];
							auto object2 = prioritised_results[j];
							upixel[0] = static_cast<float>((prioritised_results[i].bottom_left.x + prioritised_results[i].bottom_right.x) / 2);
							upixel[1] = static_cast<float>((prioritised_results[i].bottom_left.y + prioritised_results[i].top_left.y) / 2);
							vpixel[0] = static_cast<float>((prioritised_results[j].bottom_left.x + prioritised_results[j].bottom_right.x) / 2);
							vpixel[1] = static_cast<float>((prioritised_results[j].bottom_left.y + prioritised_results[j].top_left.y) / 2);
							rs2_deproject_pixel_to_point(upoint, &intr, upixel, prioritised_results[i].distance);
							rs2_deproject_pixel_to_point(vpoint, &intr, vpixel, prioritised_results[j].distance);

							float vertical_distance = sqrt(pow(upoint[0] - vpoint[0], 2.f) + pow(upoint[1] - vpoint[1], 2.f) + pow(upoint[2] - vpoint[2], 2.f))
								;
						
							SPDLOG_INFO("Vertical distance between object {} and {}: {}", prioritised_results[i].name, prioritised_results[j].name, vertical_distance);
							SPDLOG_INFO("Overlap between these objects: {}", overlap);
							if ((object1.name == "chair" && object2.name == "person") || (object1.name == "person" && object2.name == "chair")) {
								if (vertical_distance < 0.5 && overlap > 0.7) {
									SPDLOG_INFO("Person sitting on a chair");
								}
								else {
									SPDLOG_INFO("Person NOT on a chair");

								}
							}

						}
					}

				}*/
				//SPDLOG_INFO("Sending items to API");
				//api_controller_.new_items(prioritised_results);

				//SPDLOG_INFO("Sending frames to output stream");


				//output_stream_controller_.stream_to_windows(
				//	data.get_depth_frame().apply_filter(color_map), depth_matrix,
				//	data.get_color_frame(), color_matrix,
				//	prioritised_results);
