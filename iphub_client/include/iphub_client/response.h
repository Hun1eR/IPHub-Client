// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <amxx/amx.h>
#include <nlohmann/json.hpp> // I have no idea why, but std::regex not working on fucking Linux.
#include <string>

/// <summary>
/// Class Response.
/// </summary>
class Response {
public:
	/// <summary>
	/// </summary>
	Response() : status_(-1) {}

	/// <summary>
	/// </summary>
	Response(const cell status, const std::string& content) : status_(status)
	{
		json_ = nlohmann::json::parse(content, nullptr, false);
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] cell get_status() const
	{
		return status_;
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] std::string get_ip() const
	{
		return std::string(json_.value("ip", std::string()));
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] std::string get_country_code() const
	{
		return std::string(json_.value("countryCode", std::string()));
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] std::string get_country_name() const
	{
		return std::string(json_.value("countryName", std::string()));
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] cell get_asn() const
	{
		return json_.value("asn", 0);
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] std::string get_isp() const
	{
		return std::string(json_.value("isp", std::string()));
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] cell get_block() const
	{
		return json_.value("block", -1);
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] std::string get_error() const
	{
		return std::string(json_.value("error", std::string()));
	}

private:
	/// <summary>
	/// </summary>
	cell status_;

	/// <summary>
	/// </summary>
	nlohmann::json json_{};
};
