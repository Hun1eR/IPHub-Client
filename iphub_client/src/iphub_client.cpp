// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <iphub_client/iphub_client.h>
#include <iphub_client/background_worker.h>
#include <iphub_client/request.h>
#include <iphub_client/request_observer.h>
#include <iphub_client/response.h>
#include <httprequest/httprequest.h>
#include <chrono>
#include <deque>
#include <list>
#include <stdexcept>
#include <thread>

using namespace std;
using namespace std::chrono;

/// <summary>
/// </summary>
static deque<Request> g_request_queue{};

/// <summary>
/// </summary>
static list<RequestObserver*> g_observer_list{};

/// <summary>
/// </summary>
static Response send_request(const Request& request)
{
	http::Request http_request(string("http://v2.api.iphub.info/ip/") + request.ip());
	http::Response http_response;

	const vector<string> headers =
	{
		"Accept: application/json",
		"Accept-Charset: utf-8",
		string("X-Key: ") + request.key()
	};

	string error;
	auto attempts = request.attempts();

	while (attempts-- > 0) {
		try {
			http_response = http_request.send("GET", "", headers);

			if (http_response.status != 200 && http_response.status != 429)
				throw runtime_error(""); // Exceptions doesn't work correctly on Linux here.

			return Response(http_response.status, std::string(http_response.body.begin(), http_response.body.end()));
		}
		catch (...) {
			error = "Failed to send request. Status code: " + to_string(http_response.status);

			if (attempts) {
				const auto delay = request.delay();
				this_thread::sleep_for(milliseconds(delay));
			}
		}
	}

	return Response(http_response.status, string(R"({"error":")") + error + "\"}");
}

/// <summary>
/// </summary>
void IpHubClient::enqueue_task(const Request& request)
{
	BackgroundWorker::enqueue_task<Response>([](future<Response>* future)
	{
		on_response_received(future->get());
	}, send_request, request);
}

/// <summary>
/// </summary>
void IpHubClient::enqueue_request(const Request& request)
{
	const auto send_now = g_request_queue.empty();
	g_request_queue.push_back(request);

	if (send_now) {
		notify_request_in_progress(request);
		enqueue_task(request);
	}
	else {
		notify_request_queued(request);
	}
}

/// <summary>
/// </summary>
const Request* IpHubClient::find_request(const cell id)
{
	for (const auto& request : g_request_queue) {
		if (request == id)
			return &request;
	}

	return nullptr;
}

/// <summary>
/// </summary>
void IpHubClient::clear_request_queue()
{
	g_request_queue.clear();
}

/// <summary>
/// </summary>
void IpHubClient::add_observer(RequestObserver* observer)
{
	for (const auto e : g_observer_list) {
		if (e == observer)
			return;
	}

	g_observer_list.push_back(observer);
}

/// <summary>
/// </summary>
void IpHubClient::remove_observer(RequestObserver* observer)
{
	g_observer_list.remove(observer);
}

/// <summary>
/// </summary>
void IpHubClient::on_response_received(const Response& response)
{
	if (g_request_queue.empty())
		return;

	notify_response_received(g_request_queue.front(), response);
	g_request_queue.pop_front();

	if (g_request_queue.empty())
		return;

	const auto& request = g_request_queue.front();
	notify_request_in_progress(request);
	enqueue_task(request);
}

/// <summary>
/// </summary>
void IpHubClient::notify_request_queued(const Request& request)
{
	for (const auto observer : g_observer_list)
		observer->request_queued(request);
}

/// <summary>
/// </summary>
void IpHubClient::notify_request_in_progress(const Request& request)
{
	for (const auto observer : g_observer_list)
		observer->request_in_progress(request);
}

/// <summary>
/// </summary>
void IpHubClient::notify_response_received(const Request& request, const Response& response)
{
	for (const auto observer : g_observer_list)
		observer->response_received(request, response);
}
