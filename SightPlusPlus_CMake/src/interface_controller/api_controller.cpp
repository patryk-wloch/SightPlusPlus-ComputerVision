// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include "api_controller.hpp"

#include <spdlog/spdlog.h>
#include <tbb/parallel_for_each.h>

// Legacy code - Sight++ Gen 1

void ApiController::add_user(ApiUserInterface& user)
{
	api_users_.push_back(&user);
}

//void ApiController::new_items(const std::vector<ClassificationItem>& results)
//{
//	SPDLOG_INFO("Sending items to API users");
//
//	auto time = std::clock();
//
//	SPDLOG_INFO("Testing Time counter, time is {} outside loop", std::to_string(time - duration));
//	
//		for (auto&& item : results)
//		{
//			
//				tbb::parallel_for_each(
//					api_users_.begin(),
//					api_users_.end(),
//					[&](ApiUserInterface* api_user)
//					{
//						api_user->new_item(item);
//					}
//				);
//			
//		}
//		duration = clock();
//	
//}
