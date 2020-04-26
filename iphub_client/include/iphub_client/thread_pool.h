// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <type_traits>

#ifdef _WIN32
#include <thread>
#else // I'm not sure why, but std::thread crashes server on Linux. (-_-)
#include <pthread.h>
#endif

/// <summary>
/// Class ThreadPool.
/// </summary>
class ThreadPool {
	friend void* thread_pool_work(void* arg);

public:
	/// <summary>
	/// </summary>
	explicit ThreadPool(const std::size_t num_threads = 0)
		: num_threads_(num_threads) {}

	/// <summary>
	/// </summary>
	~ThreadPool()
	{
		stop();
	}

	/// <summary>
	/// </summary>
	void start();

	/// <summary>
	/// </summary>
	void stop(bool join = true);

	/// <summary>
	/// </summary>
	[[nodiscard]] bool is_running() const
	{
		return !stop_;
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] auto get_num_threads() const
	{
		return num_threads_;
	}

	/// <summary>
	/// </summary>
	template <typename TFunc, typename... TArgs>
	decltype(auto) enqueue(TFunc&& func, TArgs&&... args);

	/// <summary>
	/// <para>Move constructor.</para>
	/// </summary>
	ThreadPool(ThreadPool&&) = delete;

	/// <summary>
	/// <para>Copy constructor.</para>
	/// </summary>
	ThreadPool(const ThreadPool&) = delete;

	/// <summary>
	/// <para>Move assignment operator.</para>
	/// </summary>
	ThreadPool& operator=(ThreadPool&&) = delete;

	/// <summary>
	/// <para>Copy assignment operator.</para>
	/// </summary>
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
	/// <summary>
	/// </summary>
	std::size_t num_threads_;

	/// <summary>
	/// </summary>
	std::atomic_bool stop_{true};

	/// <summary>
	/// </summary>
	std::mutex mutex_{};

	/// <summary>
	/// </summary>
	std::condition_variable cv_{};

	/// <summary>
	/// </summary>
	std::deque<std::function<void()>> tasks_{};

#ifdef _WIN32
	/// <summary>
	/// </summary>
	std::vector<std::thread> workers_{};
#else
	/// <summary>
	/// </summary>
	std::vector<pthread_t> workers_{};
#endif
};

template <typename TFunc, typename... TArgs>
decltype(auto) ThreadPool::enqueue(TFunc&& func, TArgs&&... args)
{
	using return_type = std::invoke_result_t<std::decay_t<TFunc>, TArgs...>;

	auto task = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<TFunc>(func), std::forward<TArgs>(args)...)
	);

	auto future = std::make_shared<std::future<return_type>>(task->get_future());
	{
		std::lock_guard<std::mutex> lock(mutex_);

		tasks_.push_back([task] { (*task)(); });

		if (is_running())
			cv_.notify_one();
	}

	return future;
}
