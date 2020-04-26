// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <amxx/api.h>
#include <iphub_client/background_worker.h>
#include <iphub_client/cvars.h>
#include <iphub_client/forwards.h>
#include <iphub_client/iphub_client.h>
#include <iphub_client/logger.h>
#include <metamod/api.h>
#include <metamod/gamedll_hooks.h>

/// <summary>
/// </summary>
static Logger* g_logger = nullptr;

/// <summary>
/// </summary>
static Forwards* g_forwards = nullptr;

/// <summary>
/// </summary>
void on_server_deactivate()
{
	IpHubClient::clear_request_queue();
	BackgroundWorker::stop_all_tasks();

	RETURN_META(MetaResult::Ignored);
}

/// <summary>
/// </summary>
AmxxStatus on_amxx_attach()
{
	if (!register_cvars())
		return AmxxStatus::InterfaceMismatch;

	IpHubClient::clear_request_queue();
	GameDllHooks::server_deactivate(on_server_deactivate);

	g_logger = new Logger();
	IpHubClient::add_observer(g_logger);

	g_forwards = new Forwards();
	IpHubClient::add_observer(g_forwards);

	extern void add_natives();
	add_natives();

	return AmxxStatus::Ok;
}

/// <summary>
/// </summary>
void on_amxx_detach()
{
	IpHubClient::clear_request_queue();
	BackgroundWorker::stop_all_tasks(true);

	delete g_logger;
	g_logger = nullptr;

	delete g_forwards;
	g_forwards = nullptr;
}

/// <summary>
/// </summary>
void on_amxx_plugins_loaded()
{
	Forwards::register_forwards();
}
