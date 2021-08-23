// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/tracking.hpp>
#include "../classification_result.hpp"
#include "types.hpp"
#include <queue>


class ObjectTracker  {

	std::vector<tracked_object>& objects;
	std::queue<int>& free_ids;

public:

	ObjectTracker(std::vector<tracked_object, std::allocator<tracked_object>>& objects, std::queue<int> & free_ids) : objects(objects), free_ids(free_ids) {};

	void update_all_trackers(cv::UMat& color_matrix);
	bool update_tracker(tracked_object& object, cv::UMat& color_matrix);
	static cv::Ptr<cv::Tracker> create_tracker();
};

