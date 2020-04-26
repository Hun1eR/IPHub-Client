// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <amxx/amx.h>

class Request;
class Response;
class RequestObserver;

/// <summary>
/// Class IpHubClient.
/// </summary>
class IpHubClient {
public:
	/// <summary>
	/// </summary>
	static void enqueue_request(const Request& request);

	/// <summary>
	/// </summary>
	static const Request* find_request(cell id);

	/// <summary>
	/// </summary>
	static void clear_request_queue();

	/// <summary>
	/// </summary>
	static void add_observer(RequestObserver* observer);

	/// <summary>
	/// </summary>
	static void remove_observer(RequestObserver* observer);

private:
	/// <summary>
	/// </summary>
	static void enqueue_task(const Request& request);

	/// <summary>
	/// </summary>
	static void on_response_received(const Response& response);

	/// <summary>
	/// </summary>
	static void notify_request_queued(const Request& request);

	/// <summary>
	/// </summary>
	static void notify_request_in_progress(const Request& request);

	/// <summary>
	/// </summary>
	static void notify_response_received(const Request& request, const Response& response);
};
