// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <iphub_client/iphub_client.h>

class Request;
class Response;

/// <summary>
/// Class RequestObserver.
/// </summary>
class RequestObserver {
public:
	/// <summary>
	/// </summary>
	virtual ~RequestObserver()
	{
		IpHubClient::remove_observer(this);
	}

	/// <summary>
	/// </summary>
	void virtual request_queued(const Request& request) = 0;

	/// <summary>
	/// </summary>
	void virtual request_in_progress(const Request& request) = 0;

	/// <summary>
	/// </summary>
	void virtual response_received(const Request& request, const Response& response) = 0;

	/// <summary>
	/// <para>Default constructor.</para>
	/// </summary>
	RequestObserver() = default;

	/// <summary>
	/// <para>Move constructor.</para>
	/// </summary>
	RequestObserver(RequestObserver&&) = default;

	/// <summary>
	/// <para>Copy constructor.</para>
	/// </summary>
	RequestObserver(const RequestObserver&) = default;

	/// <summary>
	/// <para>Move assignment operator.</para>
	/// </summary>
	RequestObserver& operator=(RequestObserver&&) = default;

	/// <summary>
	/// <para>Copy assignment operator.</para>
	/// </summary>
	RequestObserver& operator=(const RequestObserver&) = default;
};
