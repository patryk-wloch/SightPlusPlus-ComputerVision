//// License: Apache 2.0. See LICENSE file in root directory.
//// Copyright(c) 2020 Sight++. All Rights Reserved.
//
//#pragma once
//#include <iostream>
//#include <string>
//#include <vector>
//#include <opencv2/tracking.hpp>
//#include "../classification_result.hpp"
//
//struct TrackingItem
//{
//	std::string name;
//	std::vector<double> distance;
//	std::vector<cv::Rect2d> rec;
//	cv::Ptr<cv::Tracker> tracker;
//	int id;
//	double confidence;
//	int counter;
//	int track_point;
//	bool lock;
//	double speed;
//	// the number of frames we have processed after we last saw this tracked object
//	int last_seen;
//
//	TrackingItem(const std::string name, const std::vector<double> distance, const std::vector<cv::Rect2d> rec, const int id, const double confidence, const int counter, const int track_point, const bool lock, const double speed, const int last_seen, const cv::Mat color_matrix, const cv::Rect2d init_rec) :
//		name(name), distance(distance), rec(rec), counter(counter), id(id), confidence(confidence), track_point(track_point), lock(lock), speed(speed), last_seen(last_seen)
//	{
//		tracker = cv::TrackerMOSSE::create();
//		tracker->init(color_matrix, init_rec);
//	}
//
//};
//struct ObjectTracking {
//
//	std::vector<TrackingItem> items;
//	const int track_frame = 5;
//	std::vector<cv::Rect2d> update_items;
//	std::vector<double> frame_time;
//	std::vector<ClassificationItem> result_objects;
//	
//	/// <summary>
//	/// update the position for tracked objects based on current frame
//	/// </summary>
//	/// <param name="color_matrix">input current color frame</param>
//	void frame_update(cv::Mat color_matrix);
//
//	/// <summary>
//	/// remove redundant objects, calculate speed and return result dataset
//	/// </summary>
//	/// <param name="stamp">number of clock ticks elapsed since the program was launched for speed calculation</param>
//	/// <returns>the classification result for prioritiser</returns>
//	std::vector<ClassificationItem> post_process(double stamp);
//
//	/// <summary>
//	/// check whether the current object is a new untracked object
//	/// if the object has been tracked, update its properties
//	/// if the object is new untracked object, add it into the tracked objects
//	/// </summary>
//	/// <param name="color_matrix">color frame for tracker initialization</param>
//	/// <param name="object_rec">the position of object identified in current frame</param>
//	/// <param name="name">the name of the identified object</param>
//	/// <param name="distance">the distance of the identified object</param>
//	/// <param name="confidence">the confidence of the identified object</param>
//	void object_check(const cv::Mat color_matrix, cv::Rect2d object_rec, std::string name, double distance, double confidence);
//
//};