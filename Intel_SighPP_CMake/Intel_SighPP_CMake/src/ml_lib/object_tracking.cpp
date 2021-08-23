// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include "object_tracking.hpp"


void ObjectTracker::update_all_trackers(cv::UMat & color_matrix) {

	for (unsigned int i = 0; i < objects.size(); i++) {
		auto& object = objects[i];

		bool update_success = update_tracker(object, color_matrix);
		
		if (object.second.no_rc_counter > 2 * TARGET_FPS) {
			free_ids.push(object.second.id);
			objects.erase(objects.begin() + i);

		}

		else {
			object.second.no_rc_counter++;
		}

	}
}

bool ObjectTracker::update_tracker(tracked_object &object, cv::UMat& color_matrix) {
	
	return object.first->update(color_matrix, object.second.bounding_box);

}

cv::Ptr<cv::Tracker> ObjectTracker::create_tracker() {
	return cv::TrackerMOSSE::create();

}
