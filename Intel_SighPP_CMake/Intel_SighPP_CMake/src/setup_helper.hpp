// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"

inline std::string get_date_time_string()
{
	const auto t = spdlog::details::os::localtime();

	std::string s;
	s.append(std::to_string(t.tm_year + 1900));
	s.append(std::to_string(t.tm_mon + 1));
	s.append(std::to_string(t.tm_mday));
	s.append("T");
	s.append(std::to_string(t.tm_hour));
	s.append(std::to_string(t.tm_min));
	s.append(std::to_string(t.tm_sec));

	return s;
}

inline void setup_logging()
{
	auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
	console_sink->set_level(spdlog::level::debug);
	console_sink->set_pattern("[%Y-%m-%d %H%M%S] [%t] [%!:%#] [%oms] [%^%l%$] %v");

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/logs-" + get_date_time_string() + ".log");
	file_sink->set_level(spdlog::level::debug);
	file_sink->set_pattern("[%Y-%m-%d %H%M%S] [%t] [%!:%#] [%o] [%l] %v");

	spdlog::sinks_init_list sink_list = { console_sink, file_sink };
	set_default_logger(std::make_shared<spdlog::logger>(".", sink_list));

}