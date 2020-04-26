// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <amxx/amx.h>
#include <stdexcept>
#include <string>
#include <utility>

class RequestBuilder;

#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#endif

/// <summary>
/// Class Request.
/// </summary>
class Request {
	friend class RequestBuilder;

public:
	/// <summary>
	/// </summary>
	static RequestBuilder create();

	/// <summary>
	/// </summary>
	[[nodiscard]] cell id() const
	{
		return id_;
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] Amx* amx() const
	{
		return amx_;
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] const char* ip() const
	{
		return ip_.c_str();
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] const char* key() const
	{
		return key_.c_str();
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] cell attempts() const
	{
		return attempts_;
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] cell delay() const
	{
		return delay_;
	}

	/// <summary>
	/// </summary>
	bool operator==(const cell id) const
	{
		return this->id() == id;
	}

	/// <summary>
	/// </summary>
	bool operator!=(const cell id) const
	{
		return !(*this == id);
	}

private:
	/// <summary>
	/// </summary>
	Request() = default;

	/// <summary>
	/// </summary>
	cell id_{};

	/// <summary>
	/// </summary>
	Amx* amx_{};

	/// <summary>
	/// </summary>
	std::string ip_{};

	/// <summary>
	/// </summary>
	std::string key_{};

	/// <summary>
	/// </summary>
	cell attempts_{3};

	/// <summary>
	/// </summary>
	cell delay_{1000};
};

#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

/// <summary>
/// Class RequestBuilder.
/// </summary>
class RequestBuilder {
public:
	/// <summary>
	/// </summary>
	RequestBuilder& amx(Amx* amx)
	{
		request_.amx_ = amx;
		return *this;
	}

	/// <summary>
	/// </summary>
	RequestBuilder& ip(const char* ip)
	{
		request_.ip_ = ip;
		return *this;
	}

	/// <summary>
	/// </summary>
	RequestBuilder& key(const char* key)
	{
		request_.key_ = key;
		return *this;
	}

	/// <summary>
	/// </summary>
	RequestBuilder& attempts(cell attempts)
	{
		if (attempts < 1)
			attempts = 1;

		if (attempts > 5)
			attempts = 5;

		request_.attempts_ = attempts;

		return *this;
	}

	/// <summary>
	/// </summary>
	RequestBuilder& delay(cell delay)
	{
		if (delay < 500)
			delay = 500;

		if (delay > 5000)
			delay = 5000;

		request_.delay_ = delay;

		return *this;
	}

	/// <summary>
	/// </summary>
	operator Request&&()
	{
		if (!request_.amx_)
			throw std::runtime_error("Amx is null.");

		if (request_.key_.empty())
			throw std::runtime_error("Empty IPHub API key provided.");

		if (request_.ip_.empty())
			throw std::runtime_error("Empty IP address provided.");

		static auto id = 0;
		request_.id_ = ++id;

		return std::move(request_);
	}

private:
	/// <summary>
	/// </summary>
	Request request_{};
};

inline RequestBuilder Request::create()
{
	return RequestBuilder();
}
