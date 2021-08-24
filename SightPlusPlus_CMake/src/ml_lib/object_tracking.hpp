// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/tracking.hpp>
#include "../config.hpp"
#include <queue>
#include <tbb/concurrent_queue.h>
#include <tbb/parallel_for.h>


class ObjectTracker  {

	tbb::concurrent_queue<int>& free_ids;

public:

	ObjectTracker(tbb::concurrent_queue<int> & free_ids) : free_ids(free_ids) {};

	void update_all_trackers(std::vector<tracked_object> & objects, cv::UMat& color_matrix);
	bool update_tracker(tracked_object& object, cv::UMat& color_matrix);
	static cv::Ptr<cv::Tracker> create_tracker();
};

