// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include "object_tracking.hpp"
#include <math.h>

// Function for updating all trackers, it also updates objects' distances and their speeds

std::string ObjectTracker::tracker = "MOSSE";

ObjectTracker::ObjectTracker(tbb::concurrent_queue<int>& free_ids, rs2_intrinsics& intr, std::string& tracker) : free_ids(free_ids), intr(intr) {
	tracker = tracker;

}

void ObjectTracker::update_all_trackers(std::vector<tracked_object> &objects, cv::UMat & color_matrix, cv::UMat & depth_matrix) {

	// Do the update calculation concurrently
	tbb::parallel_for(tbb::blocked_range<int>(0, objects.size()), [&](tbb::blocked_range<int> r) {

		// Iterate through every live object
		for (unsigned int i = r.begin(); i < r.end(); i++) {
		auto& object = objects[i];

		// Unlock object to allow for changes to its attributes
		object.second.lock = false;

		// Increment age of an object (in frames)
		object.second.age++;

		// Get 3D coordinates of the central point of the object before the tracking update
		float central_pixel_prev[2];
		float central_point_prev[3];
		central_pixel_prev[0] = static_cast<float>((object.second.bounding_box.x + object.second.bounding_box.width) / 2);
		central_pixel_prev[1] = static_cast<float>((object.second.bounding_box.y + object.second.bounding_box.height) / 2);
		rs2_deproject_pixel_to_point(central_point_prev, &intr, central_pixel_prev, object.second.distance);

		// Prepare bounding box for depth calculations
		cv::Rect2d object_depth = object.second.bounding_box;
		object_depth.width = object_depth.x > 0 ? object_depth.width : object_depth.width + object_depth.x;
		object_depth.height = object_depth.y > 0 ? object_depth.height : object_depth.height + object_depth.y;		
		object_depth.x = object_depth.x > 0 ? object_depth.x : 0;
		object_depth.y = object_depth.y > 0 ? object_depth.y : 0;
		object_depth.width = object_depth.x + object_depth.width > depth_matrix.cols ? depth_matrix.cols - object_depth.x : object_depth.width;
		object_depth.height = object_depth.y + object_depth.height > depth_matrix.rows ? depth_matrix.rows - object_depth.y : object_depth.height;
		
		// Update tracker
		bool update_success = update_tracker(object, color_matrix);

		// Get new distance to the object
		double distance = get_distance(depth_matrix(object_depth));

		// Update the distance attribute using Exponential Moving Average as stabilising mechanism
		object.second.distance = (double)(1.f - ALPHA) * object.second.distance + ALPHA * distance;

		// Get 3D coordinates of the central point of the object after the tracking update
		float central_pixel_new[2];
		float central_point_new[3];
		central_pixel_new[0] = static_cast<float>((object.second.bounding_box.x + object.second.bounding_box.width) / 2);
		central_pixel_new[1] = static_cast<float>((object.second.bounding_box.y + object.second.bounding_box.height) / 2);
		rs2_deproject_pixel_to_point(central_point_new, &intr, central_pixel_new, object.second.distance);

		// Calculate the change in euclidean location of the object between previous and current frame
		float distance_change = sqrt(pow(central_point_prev[0] - central_point_new[0], 2.f) + pow(central_point_prev[1] - central_point_new[1], 2.f) + pow(central_point_prev[2] - central_point_new[2], 2.f));

		// Calculate the speed by multiplying change in distance per frame by the number of frames per second
		double speed = distance_change * TARGET_FPS;

		// Use exponential moving average to update the object's speed and cap it at 2.5m/s (no object of interest will plausibly move faster)
		object.second.speed = std::min(2.5, speed) * ALPHA + (double)(1.f - ALPHA) * object.second.speed;


		// Remove object from the live objects vector if it hasn't been re-recognised for 3 seconds
		if (object.second.no_rc_counter > 2 * TARGET_FPS) {
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
	if (tracker.compare("MOSSE") == 0) {
		return cv::TrackerMOSSE::create();
	}
	else {
		return cv::TrackerKCF::create();
	}

}
