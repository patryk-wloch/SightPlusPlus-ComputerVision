//#include <gtest/gtest.h>
//#include "gmock/gmock.h"
//
//#include "../src/ml_lib/inference_controller.hpp"
//#include "mock_ml_model_impl.cpp"
//
//TEST(ml_controller, result_size)
//{
//	MockModelImpl model;
//	
//	ClassificationResult result{"test-impl"};
//	result.objects.emplace_back("test-item-1");
//	result.objects.emplace_back("test-item-2");
//
//	const cv::Mat matrix{};
//	
//	ON_CALL(model, do_work)
//		.WillByDefault(testing::Return(result));
//	
//	MLController controller;
//	controller.add_model(model);
//
//	controller.new_frames(matrix, matrix);
//
//	ASSERT_EQ(1, controller.get_and_clear_results().size());
//	ASSERT_EQ(0, controller.get_and_clear_results().size());
//}
//
//TEST(ml_controller, model_size)
//{
//	MockModelImpl model;
//
//	MLController controller;
//	controller.add_model(model);
//	ASSERT_EQ(1, controller.model_count());
//	controller.add_model(model);
//	ASSERT_EQ(2, controller.model_count());	
//}