//// License: Apache 2.0. See LICENSE file in root directory.
//// Copyright(c) 2020 Sight++. All Rights Reserved.
//
//#pragma once
//#include <iostream>
//#include <string>
//#include <vector>
//#include <sstream>
//#include <iomanip>
//#include "./priority_lib/priority.hpp"
//inline std::string append(std::string& s, const std::string key, const std::string value, const bool is_string_value, const bool is_last)
//{
//	s += '"';
//	s.append(key);
//	s += '"';
//	s.append(": ");
//	if (is_string_value) s.append("\"").append(value).append("\"");
//	else s.append(value);
//	if (is_last) return s;
//	return s.append(", ");
//}
//
//
//
//struct point {
//	double x;
//	double y;
//
//	point(const int x, const int y) : x(x), y(y) {}
//
//	std::string to_json()
//	{
//		std::string s = "{";
//		append(s, "x", two_deci::to_string_precise(x), true, false);
//		append(s, "y", two_deci::to_string_precise(y), true, true);
//		s.append("}");
//		return s;
//	}
//};
//
//struct ClassificationItem
//{
//
//
//	std::string name;
//	std::vector<std::string> data;
//	std::string msg;
//	std::string position;
//	double distance = 0;
//	double speed = 0;
//	int id = 0;;
//	double confidence = 0;;
//	int track_point = 0;
//	int counter = 0;;
//	point bottom_left;
//	point top_right;
//	point bottom_right;
//	point top_left;
//	Priority priority = Priority::UNDEFINED;
//	Location location = Location::CENTRE;
//	Height height = Height::CENTRE;
//
//	//Creation of tests
//	ClassificationItem(const std::string name) : name(name), bottom_left(point(0, 0)), top_right(point(0, 0)),  top_left(point(0, 0)), bottom_right(point(0, 0)) {}
//	//YOLO MODEL CONSTRUCT RESULTS. TODO: Standardise to CAFFE CONSTRUCTOR
//	ClassificationItem(const std::string name, const double distance, const point bottom_left, const point top_right) : name(name), distance(distance), bottom_left(bottom_left), top_right(top_right), top_left(point(0, 0)), bottom_right(point(0, 0)) {}
//	//CAFFE MODEL CONSTRUCT RESULTS
//	ClassificationItem(const std::string name, const double distance, const int id, const double confidence, const point bottom_left, const point top_right, const point bottom_right, const point top_left, const int counter, const int track_point, const double speed) :
//		name(name), distance(distance), confidence(confidence), id(id), bottom_left(bottom_left), top_right(top_right), bottom_right(bottom_right), top_left(top_left), counter(counter), track_point(track_point), speed(speed), priority(Priority::UNDEFINED) {}
//
//
//
//	std::string to_string() {
//		std::string s = "\n\n";
//		try
//		{
//			s.append("Printing ClassificationItem: " + name + "\n");
//			s.append("Msg" + msg + "\n");
//			s.append("ClassificationItem distance: " + std::to_string(distance) + "\n");
//			s.append("id: " + std::to_string(id) + "\n");
//			s.append("speed: " + std::to_string(speed) + "\n");
//			s.append("condifence " + std::to_string(confidence) + "\n");
//			s.append("Tracker " + std::to_string(track_point) + "\n");
//			s.append("Counter " + std::to_string(counter) + "\n");
//			s.append("Priority: " + std::to_string(static_cast<int>(priority)) + "\n");
//		}
//		catch (const std::exception&)
//		{
//			s.append("");
//		}
//
//		return s;
//	}
//
//	std::string to_json()
//	{
//		std::string s = "{";
//		append(s, "name", name, true, false);
//		append(s, "distance", two_deci::to_string_precise(distance), false, false);   
//		append(s, "msg", msg, true, false);
//		append(s, "priority", std::to_string(static_cast<int>(priority)),true,false);
//		append(s, "bottom_left", bottom_left.to_json(), false, false);
//		append(s, "top_right", top_right.to_json(), false, true);
//		s.append("}");
//
//		return s;
//	}
//
//};
//
//
//struct ClassificationResult {
////	// TODO What results to store?
////public:
////	std::string model_name;
////	std::vector<ClassificationItem> objects;
////
////	ClassificationResult(const std::string name) : model_name(name) {}
////
////	std::string to_string() {
////		std::string s = "";
////		try
////		{
////			s.append("Printing ClassificationResult: " + model_name + "\n");
////
////
////			for (size_t i = 0; i < objects.size(); i++)
////			{
////				s.append("Object " + std::to_string(i) + ": " + objects[i].to_string());
////			}
////		}
////		catch (const std::exception&)
////		{
////			s.append("");
////		}
////
////		return s;
////	}
//
//};
//
//struct PrioritisedClassificationResult
//{
//	//// TODO What results to store in a prioritised results?
//
//	//std::string model_name;
//	//std::vector<ClassificationItem> objects;
//
//	//std::string to_string() {
//	//	std::string s = "";
//	//	try
//	//	{
//	//		s.append("Printing PrioritisedClassificationResult: " + model_name + "\n");
//
//
//	//		for (size_t i = 0; i < objects.size(); i++)
//	//		{
//	//			s.append("Object " + std::to_string(i) + ": " + objects[i].to_string());
//	//		}
//	//	}
//	//	catch (const std::exception&)
//	//	{
//	//		s.append("");
//	//	}
//
//	//	return s;
//	//}
//
//};