// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <amxx/api.h>
#include <amxx/string.h>
#include <iphub_client/request.h>
#include <iphub_client/iphub_client.h>

/// <summary>
/// </summary>
static bool check_string_buffer_size(Amx* amx, const cell size, const char* func)
{
	// Internal AMXX buffer size for string retrieval.
	constexpr auto amxx_max_string_length = 16384;

	if (size < 0 || size > amxx_max_string_length) {
		constexpr auto err = "%s: Unexpected buffer size value provided (%d). Expected value in range of 0 to %d.";
		AmxxApi::log_error(amx, AmxError::Native, err, func, size, amxx_max_string_length);

		return false;
	}

	return true;
}

/// <summary>
/// </summary>
static const Request* get_request(Amx* amx, const cell id, const char* func)
{
	const auto* const request = IpHubClient::find_request(id);

	if (!request) {
		constexpr auto err = "%s: Invalid request ID provided (%d).";
		AmxxApi::log_error(amx, AmxError::Native, err, func, id);
	}

	return request;
}

/// <summary>
/// </summary>
static cell AMX_NATIVE_CALL iphub_send_request(Amx* amx, cell* params)
{
	enum Args { Count, Ip, Key, Attempts, Delay };

	try {
		auto* const ip = AmxxApi::get_amx_string(amx, params[Ip], 0);
		auto* const key = AmxxApi::get_amx_string(amx, params[Key], 1);

		const Request request = Request::create().amx(amx).ip(ip).key(key).attempts(params[Attempts]).delay(params[Delay]);
		IpHubClient::enqueue_request(request);

		return request.id();
	}
	catch (const std::exception& ex) {
		AmxxApi::log_error(amx, AmxError::Native, "%s: %s", __func__, ex.what());
		return 0;
	}
}

/// <summary>
/// </summary>
static cell AMX_NATIVE_CALL iphub_request_ip(Amx* amx, cell* params)
{
	enum Args { Count, Request, Buffer, BufferSize };

	const auto buf_size = params[BufferSize];
	const auto* const request = get_request(amx, params[Request], __func__);

	if (!request || !check_string_buffer_size(amx, buf_size, __func__))
		return 0;

	auto ip = amx::string(amx, params[Buffer], buf_size);
	ip = request->ip();

	return ip.length();
}

/// <summary>
/// </summary>
static cell AMX_NATIVE_CALL iphub_request_key(Amx* amx, cell* params)
{
	enum Args { Count, Request, Buffer, BufferSize };

	const auto buf_size = params[BufferSize];
	const auto* const request = get_request(amx, params[Request], __func__);

	if (!request || !check_string_buffer_size(amx, buf_size, __func__))
		return 0;

	auto key = amx::string(amx, params[Buffer], buf_size);
	key = request->key();

	return key.length();
}

/// <summary>
/// </summary>
static cell AMX_NATIVE_CALL iphub_request_attempts(Amx* amx, cell* params)
{
	enum Args { Count, Request };

	const auto* const request = get_request(amx, params[Request], __func__);

	return request ? request->attempts() : 0;
}

/// <summary>
/// </summary>
static cell AMX_NATIVE_CALL iphub_request_delay(Amx* amx, cell* params)
{
	enum Args { Count, Request };

	const auto* const request = get_request(amx, params[Request], __func__);

	return request ? request->delay() : 0;
}

/// <summary>
/// </summary>
static cell AMX_NATIVE_CALL iphub_request_initiator(Amx* amx, cell* params)
{
	enum Args { Count, Request };

	const auto* const request = get_request(amx, params[Request], __func__);

	return request ? AmxxApi::find_amx_script_by_amx(request->amx()) : -1;
}

/// <summary>
/// </summary>
static cell AMX_NATIVE_CALL iphub_initiated_by_me(Amx* amx, cell* params)
{
	enum Args { Count, Request };

	const auto* const request = get_request(amx, params[Request], __func__);

	return request && request->amx() == amx;
}

/// <summary>
/// </summary>
void add_natives()
{
	static const AmxNativeInfo natives[] =
	{
		{"iphub_send_request", iphub_send_request},
		{"iphub_request_ip", iphub_request_ip},
		{"iphub_request_key", iphub_request_key},
		{"iphub_request_attempts", iphub_request_attempts},
		{"iphub_request_delay", iphub_request_delay},
		{"iphub_request_initiator", iphub_request_initiator},
		{"iphub_initiated_by_me", iphub_initiated_by_me},
		{nullptr, nullptr}
	};

	AmxxApi::add_natives(natives);
}
