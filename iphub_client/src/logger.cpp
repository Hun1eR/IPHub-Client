// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <iphub_client/logger.h>
#include <amxx/api.h>
#include <iphub_client/cvars.h>
#include <iphub_client/request.h>
#include <iphub_client/response.h>

/// <summary>
/// </summary>
static const char* get_plugin_name(const Request& request)
{
	static auto empty = "";
	const auto plugin_id = AmxxApi::find_amx_script_by_amx(request.amx());

	return plugin_id == -1 ? empty : AmxxApi::get_amx_script_name(plugin_id, true);
}

/// <summary>
/// </summary>
void Logger::request_queued(const Request& request)
{
	if (!get_iphub_log())
		return;

	AmxxApi::log("[%s] Request has been added to the queue. [Request ID: %d, initiated by %s]\n",
	             AMXX_MODULE_LOG_TAG, request.id(), get_plugin_name(request));
}

/// <summary>
/// </summary>
void Logger::request_in_progress(const Request& request)
{
	if (!get_iphub_log())
		return;

	AmxxApi::log("[%s] Sending request. [Request ID: %d, initiated by %s]\n",
	             AMXX_MODULE_LOG_TAG, request.id(), get_plugin_name(request));
}

/// <summary>
/// </summary>
void Logger::response_received(const Request& request, const Response& response)
{
	if (!get_iphub_log())
		return;

	AmxxApi::log("[%s] Response received. Status code: %d. [Request ID: %d, initiated by %s]\n",
	             AMXX_MODULE_LOG_TAG, response.get_status(), request.id(), get_plugin_name(request));
}
