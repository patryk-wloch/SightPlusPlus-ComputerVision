#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "gtest/gtest.h"
#include <Set>


#include "../src/priority_lib/smart_priority.hpp" 
#include "../src/priority_lib/smart_priority.cpp"  
#include "../src/priority_lib/priority_clock.cpp"   
#include "../src/priority_lib/priority_clock.hpp"   
#include "../src/priority_lib/priority_module.cpp"
#include "../src/priority_lib/priority_module.hpp"
#include "../src/classification_result.hpp"

namespace {
	class prioTest : public ::testing::Test {

	protected:
		smart_priority* d;
		ClassificationResult clas_res{ "name" };
		protected:

		    ClassificationItem item1{"cup"};
		    ClassificationItem item2{ "hat" };
		    ClassificationItem item3{ "dog" };
		    ClassificationItem item4{ "person" };
			ClassificationItem item5{ "person" };
			ClassificationItem item6{ "bicycle" };

		    std::vector<ClassificationResult> test_vector;
		    std::vector<ClassificationItem> test_items;

			prioTest() {

			}

		virtual ~prioTest() {
		}

		virtual void SetUp() {
			d = new smart_priority("test");
			d->determine_location_markers(640, 480);
			item1.distance = 0.3;
			item1.bottom_right = *(new point(350, 150));
			item1.top_left = *(new point(190, 290));
			item1.counter = 1;
			item1.track_point = 5;
			item1.speed = 0.1;

			item2.distance = 0.7;
			item2.bottom_right = *(new point(512, 440));
			item2.top_left = *(new point(480, 470 ));
			item2.counter = 1;
			item2.track_point = 5;
			item2.speed = 0.1;

			item3.distance = 1.5;
			item3.bottom_right = *(new point(319, 10));
			item3.top_left = *(new point(140, 200));
			item3.counter = 1;
			item3.track_point = 5;
			item3.speed = 0.1;

			item4.distance = 4;
			item4.bottom_right = *(new point(120, 50));
			item4.top_left = *(new point(5, 200));
			item4.counter = 1;
			item4.track_point = 5;
			item4.speed = 0.1;


			item5.distance = 8;
			item5.bottom_right = *(new point(120, 50));
			item5.top_left = *(new point(5, 200));
			item5.counter = 1;
			item5.track_point = 5;
			item5.speed = -0.1;

			item5.distance = 8;
			item5.bottom_right = *(new point(120, 50));
			item5.top_left = *(new point(5, 200));
			item5.counter = 1;
			item5.track_point = 5;
			item5.speed = -0.1;

			item6.distance = 8;
			item6.bottom_right = *(new point(630, 50));
			item6.top_left = *(new point(550, 200));
			item6.counter = 1;
			item6.track_point = 5;
			item6.speed = 2;


			test_items.push_back(item1);
			test_items.push_back(item2);
			test_items.push_back(item3);
			test_items.push_back(item4);
			test_items.push_back(item5);
			test_items.push_back(item6);
			clas_res.model_name = "test_model";
			clas_res.objects = test_items;
			test_vector.push_back(clas_res);

		}

		virtual void TearDown() {
			delete d;
		}

	};

}
TEST_F(prioTest, TestSmartCreation) {
	std::string name = "cup";
	double distance = 0.3;
	auto data = d->run(&test_vector);
	auto check = (*data)[0];
	auto location = check.location;
	std::cout << check.to_string();
	ASSERT_EQ(name, check.name);
	ASSERT_EQ(distance, check.distance);
}

//Testing the correct location of the items. Note that
// As the list is sorted after priotisation. This list has been manually
// Worked out for test. Be careful on change as it does not match to origional list.
TEST_F(prioTest, TestLocations) {
	std::string name = "cup";
	int distance = 0.3;
	auto data = d->run(&test_vector);
	auto check = (*data)[0];
	auto location = check.location;
	EXPECT_EQ(Location::CENTRE, location);
	auto height = check.height;
	EXPECT_EQ(Height::CENTRE, height);

	check = (*data)[1];
	location = check.location;
	EXPECT_EQ(Location::IN_RIGHT, location);
	height = check.height;
	EXPECT_EQ(Height::ABOVE, height);


	check = (*data)[2];
	location = check.location;
	EXPECT_EQ(Location::IN_LEFT, location);
	height = check.height;
	EXPECT_EQ(Height::CENTRE, height);

	check = (*data)[3];
	location = check.location;
	EXPECT_EQ(Location::OUT_RIGHT, location);
	height = check.height;
	EXPECT_EQ(Height::CENTRE, height);

	check = (*data)[4];
	location = check.location;
	EXPECT_EQ(Location::OUT_LEFT, location);
	height = check.height;
	EXPECT_EQ(Height::CENTRE, height);


}


//Testing the correct priotisation of the items. Note that
// As the list is sorted after priotisation. This list has been manually
// Worked out for test. Be careful on change as it does not match to origional list.
TEST_F(prioTest, TestPriority) {
	std::string name = "cup";
	auto data = d->run(&test_vector);
	auto check = (*data)[0];
	auto priority = check.priority;
	EXPECT_EQ(Priority::URGENT, priority);

	check = (*data)[1];
	priority = check.priority;
	EXPECT_EQ(Priority::HIGH, priority);


	check = (*data)[2];
	priority = check.priority;
	EXPECT_EQ(Priority::HIGH, priority);


	check = (*data)[3];
	priority = check.priority;
	EXPECT_EQ(Priority::HIGH, priority);

	check = (*data)[4];
	priority = check.priority;
	EXPECT_EQ(Priority::MEDIUM, priority);

	check = (*data)[5];
	priority = check.priority;
	EXPECT_EQ(Priority::LOW, priority);

}
