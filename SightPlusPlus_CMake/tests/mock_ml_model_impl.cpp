#include "gmock/gmock.h"

#include "../src/ml_lib/ml_interface.hpp"

struct MockModelImpl : ModelInterface
{
	MOCK_METHOD(
		ClassificationResult,
		do_work,
		(cv::Mat color_matrix, cv::Mat depth_matrix),
		(override)
	);
};