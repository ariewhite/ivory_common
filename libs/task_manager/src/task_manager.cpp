#include <ivory/task_manager/task_manager.h>

#include <chrono>

namespace ivo
{

task_manager::task_manager(size_t threads) : threads_(threads)
{
}

task_manager::~task_manager()
{
}

void task_manager::worker_loop(std::stop_token token)
{

}

// // blocking copy
// task_manager(const task_manager &) = delete;
// task_manager *operator=(const task_manager &) = delete;

} // namespace ivo