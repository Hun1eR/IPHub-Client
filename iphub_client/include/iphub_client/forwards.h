// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <iphub_client/request_observer.h>

/// <summary>
/// Class Forwards.
/// </summary>
class Forwards final : public RequestObserver {
public:	
	/// <summary>
	/// </summary>
	static void register_forwards();

	/// <summary>
	/// </summary>
	void request_queued(const Request& request) override;

	/// <summary>
	/// </summary>
	void request_in_progress(const Request& request) override;

	/// <summary>
	/// </summary>
	void response_received(const Request& request, const Response& response) override;
};
