// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include "object_tracking.hpp"


void ObjectTracker::update_all_trackers(std::vector<tracked_object> &objects, cv::UMat & color_matrix) {

	//tbb::parallel_for(tbb::blocked_range<int>(0, objects.size()), [&](tbb::blocked_range<int> r) {


		for (unsigned int i = 0; i < objects.size(); i++) {
		auto& object = objects[i];
		object.second.lock = false;
		bool update_success = update_tracker(object, color_matrix);
		
		if (object.second.no_rc_counter > 3 * TARGET_FPS) {
			free_ids.push(object.second.id);
			objects.erase(objects.begin() + i);

		}

		else {
			object.second.no_rc_counter++;
		}

	}
		//});
}

bool ObjectTracker::update_tracker(tracked_object &object, cv::UMat& color_matrix) {
	
	return object.first->update(color_matrix, object.second.bounding_box);

}

cv::Ptr<cv::Tracker> ObjectTracker::create_tracker() {
	return cv::TrackerKCF::create();

}
