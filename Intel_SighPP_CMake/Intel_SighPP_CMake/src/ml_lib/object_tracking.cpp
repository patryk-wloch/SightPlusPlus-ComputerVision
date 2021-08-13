//// License: Apache 2.0. See LICENSE file in root directory.
//// Copyright(c) 2020 Sight++. All Rights Reserved.
//
//#include "object_tracking.hpp"
//
//void ObjectTracking::frame_update(cv::Mat color_matrix) {
//	update_items.clear();
//	for (int i = 0; i < items.size(); i++) {
//		cv::Rect2d update_rec;
//		items[i].tracker->update(color_matrix, update_rec);
//		update_items.push_back(update_rec);
//	}
//}
//
//std::vector<ClassificationItem> ObjectTracking::post_process(double stamp) {
//	result_objects.clear();
//	for (int i = 0; i < items.size(); i++) {
//		if (!items[i].lock) {
//			items[i].track_point--;
//			if (items[i].track_point <= 0) {
//				items.erase(items.begin() + i);
//			}
//		}
//		else {
//			items[i].lock = false;
//		}
//	}
//	frame_time.push_back(stamp);
//	if (frame_time.size() > track_frame + 1) {
//		frame_time.erase(frame_time.begin());
//	}
//	for (int i = 0; i < items.size(); i++) {
//		if (items[i].track_point == 5 && items[i].distance.size() >= 2) {
//			int distance_size = items[i].distance.size();
//			int time_size = frame_time.size();
//			items[i].speed = (items[i].distance[distance_size - 2] - items[i].distance[distance_size - 1]) / (frame_time[time_size - 1] - frame_time[time_size - 1 - items[i].last_seen]);
//			items[i].last_seen = 0;
//		}
//		items[i].last_seen++;
//		int rec_size = items[i].distance.size();
//		cv::Point tl = items[i].rec[rec_size - 1].tl();
//		cv::Point br = items[i].rec[rec_size - 1].br();
//		point bottom_left(tl.x, tl.y);
//		point top_right(br.x, br.y);
//		point bottom_right(br.x, tl.y);
//		point top_left(tl.x, br.y);
//		int distance_size = items[i].distance.size();
//		ClassificationItem new_result(items[i].name, items[i].distance[distance_size - 1], items[i].id, items[i].confidence, bottom_left, top_right, bottom_right, top_left, items[i].counter, items[i].track_point, items[i].speed);
//		result_objects.push_back(new_result);
//	}
//	return result_objects;
//}
//
//void ObjectTracking::object_check(const cv::Mat color_matrix, cv::Rect2d object_rec, std::string name, double distance, double confidence) {
//	bool add_object = true;
//
//	for (int i = 0; i < update_items.size(); i++) {
//		cv::Rect overlap = static_cast<cv::Rect2d>(object_rec) & update_items[i];
//
//		/// <summary>
//		/// the rectangle of current object overlaps the rectangle of tracked object 
//		/// they have the same name
//		/// tracked object has not been updated for this model
//		/// </summary>
//
//		if (overlap.area() > 0
//			&& name == items[i].name
//			&& items[i].lock == false) {
//			add_object = false;
//			items[i].counter++;
//			items[i].track_point = 5;
//			items[i].lock = true;
//			items[i].distance.push_back(distance);
//			if (items[i].distance.size() > track_frame) {
//				items[i].distance.erase(items[i].distance.begin());
//			}
//			items[i].rec.push_back(object_rec);
//			if (items[i].rec.size() > track_frame) {
//				items[i].rec.erase(items[i].rec.begin());
//			}
//			break;
//		}
//
//	}
//
//	if (add_object) {
//		bool object_id[100];
//		for (int j = 0; j < 100; j++) {
//			object_id[j] = true;
//		}
//		std::vector<double> current_distance;
//		std::vector<cv::Rect2d> current_rec;
//		current_distance.push_back(distance);
//		current_rec.push_back(object_rec);
//		int new_id = 1;
//		for (int i = 0; i < items.size(); i++) {
//			if (items[i].name == name) {
//				object_id[items[i].id] = false;
//			}
//		}
//		for (int j = 1; j < 100; j++) {
//			if (object_id[j]) {
//				new_id = j;
//				break;
//			}
//
//		}
//		TrackingItem new_object(name, current_distance, current_rec, new_id, confidence, 1, 5, true, 0, 0, color_matrix, object_rec);
//		items.push_back(new_object);
//
//	}
//}