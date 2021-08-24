
#pragma once
#include <opencv2/tracking/tracking.hpp>

struct DetectionResult
{
	int label = 0;
	double first_distance = 0;
	double distance = 0;
	int id = 0;
	cv::Rect2d bounding_box;
	int age = 0;
	int no_rc_counter = 0;
};

const float ALPHA = 0.3;
const float COMP_SCALE = 0.5;
const std::string PATH_TO_MODEL = "models/ssd300.xml";
const std::string VINO_CONFIG = "GPU";
const int DEC_FILTER = 2;
const int SPAT_FILTER = 3;
const int HOLE_FILTER = 1;
const int MAX_OBJECT_COUNT = 20;
const int TARGET_FPS = 30;
const bool DEPTH = true;
const int CAMERA_FPS = 30;
const float WH_RATIO = 1.3333;

typedef std::pair <cv::Ptr<cv::Tracker>, DetectionResult> tracked_object;

	