// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <iphub_client/thread_pool.h>
#include <utility>

#ifdef _WIN32
/// <summary>
/// </summary>
static void join_threads(std::vector<std::thread>& workers)
{
	for (auto& worker : workers)
		worker.join();
}

/// <summary>
/// </summary>
static void detach_threads(std::vector<std::thread>& workers)
{
	for (auto& worker : workers)
		worker.detach();
}
#else
/// <summary>
/// </summary>
static void join_threads(const std::vector<pthread_t>& workers)
{
	for (auto worker : workers)
		pthread_join(worker, nullptr);
}

/// <summary>
/// </summary>
static void detach_threads(const std::vector<pthread_t>& workers)
{
	for (auto worker : workers)
		pthread_detach(worker);
}
#endif

/// <summary>
/// </summary>
void* thread_pool_work(void* arg)
{
	const auto thread_pool = static_cast<ThreadPool*>(arg);

	while (!thread_pool->stop_.load(std::memory_order_relaxed)) {
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(thread_pool->mutex_);

			thread_pool->cv_.wait(lock, [thread_pool]
			{
				return thread_pool->stop_.load(std::memory_order_relaxed) || !thread_pool->tasks_.empty();
			});

			if (thread_pool->stop_.load(std::memory_order_relaxed))
				return nullptr;

			task = std::move(thread_pool->tasks_.front());
			thread_pool->tasks_.pop_front();
		}

		task();
	}

	return nullptr;
}

/// <summary>
/// </summary>
void ThreadPool::start()
{
	if (is_running())
		return;

	stop_ = false;

	if (!num_threads_) {
		const auto concurrency = std::thread::hardware_concurrency();

		if (concurrency <= 0) {
			num_threads_ = 4;
		}
		else if (concurrency > 12) {
			num_threads_ = 12;
		}
		else {
			num_threads_ = concurrency;
		}
	}

#ifdef _WIN32
	for (std::size_t i = 0; i < num_threads_; ++i) {
		workers_.emplace_back(thread_pool_work, this);
	}
#else
	for (std::size_t i = 0; i < num_threads_; ++i) {
		pthread_t worker;
		
		if (pthread_create(&worker, nullptr, thread_pool_work, this))
			break; // TODO: handle error.
		
		workers_.push_back(worker);
	}
#endif
}

/// <summary>
/// </summary>
void ThreadPool::stop(const bool join)
{
	if (!is_running())
		return;

	stop_ = true;
	cv_.notify_all();

	if (join) {
		join_threads(workers_);
	}
	else {
		detach_threads(workers_);
	}

	tasks_.clear();
	workers_.clear();
}
