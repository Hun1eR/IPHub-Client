// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <iphub_client/forwards.h>
#include <amxx/api.h>
#include <iphub_client/request.h>
#include <iphub_client/response.h>

/// <summary>
/// </summary>
static int g_in_progress = -1;

/// <summary>
/// </summary>
static int g_request_queued = -1;

/// <summary>
/// </summary>
static int g_response_received = -1;

/// <summary>
/// </summary>
struct IpHubData {
	/// <summary>
	/// </summary>
	cell ip[16]{};

	/// <summary>
	/// </summary>
	cell country_code[4]{};

	/// <summary>
	/// </summary>
	cell country_name[32]{};

	/// <summary>
	/// </summary>
	cell asn{};

	/// <summary>
	/// </summary>
	cell isp[64]{};

	/// <summary>
	/// </summary>
	cell block{-1};

	/// <summary>
	/// </summary>
	cell error[256]{};
};

/// <summary>
/// </summary>
template <typename TMember>
static void set_iphub_data(IpHubData& data, TMember member, const std::string& value)
{
	std::size_t i = 0;
	const auto size = sizeof(data.*member) / sizeof(cell) - 1;

	while (value[i] && i < size) {
		(data.*member)[i] = static_cast<unsigned char>(value[i]);
		++i;
	}

	(data.*member)[i] = 0;
}

/// <summary>
/// </summary>
void Forwards::register_forwards()
{
	g_request_queued = AmxxApi::register_forward("iphub_request_queued",
		ForwardExecType::Ignore, ForwardParam::Cell, ForwardParam::Done);

	g_in_progress = AmxxApi::register_forward("iphub_request_in_progress",
		ForwardExecType::Ignore, ForwardParam::Cell, ForwardParam::Done);

	g_response_received = AmxxApi::register_forward("iphub_response_received",
		ForwardExecType::Stop, ForwardParam::Cell, ForwardParam::Array, ForwardParam::Cell, ForwardParam::Done);
}

/// <summary>
/// </summary>
void Forwards::request_queued(const Request& request)
{
	AmxxApi::execute_forward(g_request_queued, request.id());
}

/// <summary>
/// </summary>
void Forwards::request_in_progress(const Request& request)
{
	AmxxApi::execute_forward(g_in_progress, request.id());
}

/// <summary>
/// </summary>
void Forwards::response_received(const Request& request, const Response& response)
{
	static IpHubData data;

	data.asn = response.get_asn();
	data.block = response.get_block();

	set_iphub_data(data, &IpHubData::ip, response.get_ip());
	set_iphub_data(data, &IpHubData::country_code, response.get_country_code());
	set_iphub_data(data, &IpHubData::country_name, response.get_country_name());
	set_iphub_data(data, &IpHubData::isp, response.get_isp());
	set_iphub_data(data, &IpHubData::error, response.get_error());

	auto array_id = AmxxApi::prepare_cell_array(reinterpret_cast<cell*>(&data), sizeof data / sizeof(cell));
	AmxxApi::execute_forward(g_response_received, request.id(), array_id, response.get_status());
}
