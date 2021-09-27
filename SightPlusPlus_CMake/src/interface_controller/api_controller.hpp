// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include <vector>
#include <ctime>

#include "api_user_interface.hpp"
 

// Legacy code - Sight++ Gen 1
class ApiController
{
	std::vector<ApiUserInterface*> api_users_;
	int duration = std::clock();
public:
	
	ApiController() = default;
	
	/// <summary>
	/// Add output API implementation to list of current users.
	/// </summary>
	/// <param name="user">User to add to current users.</param>
	void add_user(ApiUserInterface& user);

	/// <summary>
	/// Receives new classification result items and sends them to connected API users.
	/// </summary>
	/// <param name="results">Items to send to API users</param>
	//void new_items(const std::vector<ClassificationItem>& results);

};