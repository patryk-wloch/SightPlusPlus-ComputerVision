//// License: Apache 2.0. See LICENSE file in root directory.
//// Copyright(c) 2020 Sight++. All Rights Reserved.
//
//#pragma once
//
//#include <opencv2/dnn.hpp>
//#include "librealsense2/rs.hpp"
//#include "../classification_result.hpp"
//
//struct ModelInterface {
//
//	size_t inWidth = 640;
//	size_t inHeight = 480;
//	float WHRatio = inWidth / (float)inHeight;
//	float inScaleFactor = 0.007843f;
//	float meanVal = 127.5;
//	float confidence_threshold = 0.5f;
//
//  virtual ~ModelInterface() = default;
//
//	/// <summary>
// 	/// Function to identify objects and return the resutls
// 	/// (obejct class names, relative location, distance and confidence)
// 	/// </summary>
// 	/// <param name="color_matrix">color matrix obtained from the camera</param>
// 	/// <param name="depth_matrix">depth matrix obtained from the camera</param>
//	virtual ClassificationResult do_work(cv::Mat color_matrix, cv::Mat depth_matrix) = 0;
//
//	/// <summary>
// 	/// Function to set frame's resolution in object detection networks
// 	/// </summary>
// 	/// <param name="width">width of the frame resolution</param>
// 	/// <param name="height">height of the frame resolution</param>
//	void set_resolution(size_t width, size_t height){
//		inWidth = width;
//		inHeight = height;
//	}
//	/// <summary>
// 	/// Function to set confidence threshold in object detection networks
// 	/// </summary>
// 	/// <param name="threshold">confidence threshold</param>
//	void set_confidence_threshold(float threshold){
//		confidence_threshold = threshold;
//	}
//
//};
