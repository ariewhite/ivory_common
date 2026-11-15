#include <ivory/task_manager/task_manager.h>

#include <chrono>

namespace ivo
{

task_manager::task_manager(size_t threads) : threads_(threads)
{
    if (threads_ <= 0)
    {
        throw std::runtime_error("task_manager(): threads count me be positive integer");
    }
}

task_manager::~task_manager()
{
}

void task_manager::worker_loop(std::stop_token token)
{
}


} // namespace ivo