// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <iphub_client/background_worker.h>
#include <metamod/api.h>
#include <metamod/gamedll_hooks.h>
#include <vector>

/// <summary>
/// </summary>
static std::vector<WorkerTask> g_running_tasks{};

/// <summary>
/// </summary>
static bool g_stop{true};

/// <summary>
/// </summary>
ThreadPool BackgroundWorker::thread_pool_{};

/// <summary>
/// </summary>
std::size_t BackgroundWorker::tasks_count()
{
	return g_running_tasks.size();
}

/// <summary>
/// </summary>
void BackgroundWorker::stop_all_tasks(const bool join)
{
	g_stop = true;
	g_running_tasks.clear(); // Should be called before thread_pool_.clear_tasks();
	thread_pool_.stop(join);
}

/// <summary>
/// </summary>
void BackgroundWorker::start()
{
	if (g_stop) {
		g_stop = false;
		thread_pool_.start();
	}

	GameDllHooks::start_frame(on_start_frame_post, true);
}

/// <summary>
/// </summary>
void BackgroundWorker::add_task(const std::function<bool()>& is_task_complete, const std::function<void()>& callback)
{
	g_running_tasks.emplace_back(is_task_complete, callback);
}

/// <summary>
/// </summary>
void BackgroundWorker::on_start_frame_post()
{
	static unsigned int frame = 0;

	if (!g_stop && ++frame % 30) // Check tasks every 30 frames.
		RETURN_META(MetaResult::Ignored);

	auto it = g_running_tasks.begin();
	const auto& end = g_running_tasks.end();

	while (!g_stop && it != end) {
		if (!it->is_complete()) {
			++it;
			continue;
		}

		const auto task = std::move(*it);
		g_running_tasks.erase(it);

		task.execute_callback();
		break; // Don't execute many callbacks in one frame.
	}

	if (g_stop || g_running_tasks.empty())
		GameDllHooks::start_frame(nullptr, true);

	RETURN_META(MetaResult::Ignored);
}
