
#pragma once
#include <opencv2/tracking/tracking.hpp>
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>

struct DetectionResult
{
	int label = 0;
	double first_distance = 0;
	double distance = 0;
	int id = 0;
	cv::Rect2d bounding_box;
	int age = 0;
	int no_rc_counter = 0;
	cv::Scalar color;
	bool lock = false;
	cv::Rect2d first_bounding_box;
	double speed = 0;
};

const float ALPHA = 0.1;
const float COMP_SCALE = 0.33;
const int DEC_FILTER = 2;
const int SPAT_FILTER = 3;
const int HOLE_FILTER = 1;
const int MAX_OBJECT_COUNT = 20;
const int TARGET_FPS = 5;
const bool DEPTH = true;
const int CAMERA_FPS = 30;
const float WH_RATIO = 1.3333;

typedef std::pair <cv::Ptr<cv::Tracker>, DetectionResult> tracked_object;


