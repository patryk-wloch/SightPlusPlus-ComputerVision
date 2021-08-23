
struct DetectionResult
{
	int label = 0;
	double first_distance = 0;
	double distance = 0;
	int id = 0;
	cv::Rect2d double_bounding_box;
	cv::Rect2d bounding_box;
	int age = 0;
	int no_rc_counter = 0;
};

const float ALPHA = 0.3;
const std::string path_to_model = "models/ssd_mobilenet_v2_coco.xml";
const std::string vino_config = "MULTI";
const int MAX_OBJECT_COUNT = 10;
const int TARGET_FPS = 15;

typedef std::pair <cv::Ptr<cv::Tracker>, DetectionResult> tracked_object;

	