#pragma once

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
const std::string PATH_TO_MODEL = "models/ssd_mobilenet_v2_coco.xml";
const std::string VINO_CONFIG = "GPU";
const int MAX_OBJECT_COUNT = 20;
const int TARGET_FPS = 20;
const bool DEPTH = false;

typedef std::pair <cv::Ptr<cv::Tracker>, DetectionResult> tracked_object;

	