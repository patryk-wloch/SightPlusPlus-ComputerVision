#include <gtest/gtest.h>

#include "../src/classification_result.hpp"


TEST(JSON, Valid)
{
	ClassificationItem item{ "test1" };

	ASSERT_EQ(
		item.to_json(), 
		"{\"name\": \"test1\", \"distance\": 0, \"msg\": \"\", \"priority\": \"0\", \"bottom_left\": {\"x\": \"0\", \"y\": \"0\"}, \"top_right\": {\"x\": \"0\", \"y\": \"0\"}}"
	);
}
