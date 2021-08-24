// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Sight++. All Rights Reserved.

#include "api_user_interface.hpp"
#include <set>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "spdlog/spdlog.h"

/// <summary>
/// Based on WebSocket++ Simple Broadcast Server.
/// </summary>
class BroadcastServer
{
	std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections_;
	websocketpp::server<websocketpp::config::asio> server_;

public:

	BroadcastServer()
	{
		SPDLOG_INFO("Creating BroadcastServer");
		server_.init_asio();

		server_.set_open_handler(websocketpp::lib::bind(&BroadcastServer::on_open, this, websocketpp::lib::placeholders::_1));
		server_.set_close_handler(websocketpp::lib::bind(&BroadcastServer::on_close, this, websocketpp::lib::placeholders::_1));
		server_.set_message_handler(websocketpp::lib::bind(&BroadcastServer::on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
	}

	void run(int port)
	{
		SPDLOG_INFO("Running BroadcastServer");
		server_.listen(port);
		server_.start_accept();
		server_.run();
		SPDLOG_INFO("Running BroadcastServer");
	}

	void on_open(const websocketpp::connection_hdl handle) {
		connections_.insert(handle);
	}

	void on_close(const websocketpp::connection_hdl handle) {
		connections_.erase(handle);
	}

	void on_message(const websocketpp::connection_hdl handle, const websocketpp::server<websocketpp::config::asio>::message_ptr message) {
		for (const auto& connection : connections_)
		{
			server_.send(connection, message);
		}
	}

	void send(const std::string& message)
	{
		for (const auto& connection : connections_)
		{
			server_.send(connection, message, websocketpp::frame::opcode::TEXT);
		}
	}
};

struct ApiWebSocketImpl : ApiUserInterface
{
	/*Priority minimum_priority;
	BroadcastServer server;

	ApiWebSocketImpl(int port, Priority minimum_priority) : minimum_priority(minimum_priority)
	{
		websocketpp::lib::thread([s = &server, port] { s->run(port); }).detach();
	}

	void new_item(ClassificationItem item) override
	{
		SPDLOG_INFO("MinPriority: {} vs. ItemPriority: {}", static_cast<int>(minimum_priority), static_cast<int>(item.priority));
		if (item.priority >= minimum_priority)
		{
			SPDLOG_INFO("Sending item: {}", item.to_json());
			server.send(item.to_json());
		}
	}*/
};
