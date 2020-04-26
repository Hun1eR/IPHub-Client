// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <iphub_client/thread_pool.h>
#include <chrono>
#include <cstddef>
#include <utility>

/// <summary>
/// Struct WorkerTask
/// </summary>
class WorkerTask {
public:
	/// <summary>
	/// </summary>
	WorkerTask(std::function<bool()> is_complete, std::function<void()> callback)
		: is_complete_(std::move(is_complete)), callback_(std::move(callback)) {}

	/// <summary>
	/// </summary>
	[[nodiscard]] bool is_complete() const
	{
		return is_complete_();
	}

	/// <summary>
	/// </summary>
	void execute_callback() const
	{
		callback_();
	}

private:
	/// <summary>
	/// </summary>
	std::function<bool()> is_complete_;

	/// <summary>
	/// </summary>
	std::function<void()> callback_;
};

/// <summary>
/// Class BackgroundWorker.
/// </summary>
class BackgroundWorker {
	/// <summary>
	/// </summary>
	static ThreadPool thread_pool_;

public:
	/// <summary>
	/// </summary>
	template <typename TRet, typename TFunc, typename... TArgs>
	static void enqueue_task(const std::function<void(std::future<TRet>*)>& callback, TFunc&& task, TArgs&&... args);

	/// <summary>
	/// </summary>
	static std::size_t tasks_count();

	/// <summary>
	/// </summary>
	static void stop_all_tasks(bool join = false);

private:
	/// <summary>
	/// </summary>
	static void start();

	/// <summary>
	/// </summary>
	static void add_task(const std::function<bool()>& is_task_complete, const std::function<void()>& callback);

	/// <summary>
	/// </summary>
	static void on_start_frame_post();
};

template <typename TRet, typename TFunc, typename ... TArgs>
void BackgroundWorker::enqueue_task(const std::function<void(std::future<TRet>*)>& callback, TFunc&& task, TArgs&&... args)
{
	const auto future = thread_pool_.enqueue(std::forward<TFunc>(task), std::forward<TArgs>(args)...);

	const auto is_task_complete = [future]
	{
		return future->wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	};

	const auto on_task_complete = [callback, future]
	{
		callback(future.get());
	};

	add_task(is_task_complete, on_task_complete);
	start();
}
