/// @brief Task manager

#pragma once

#include <thread>
#include <vector>

namespace ivo
{

class task_manager
{
public:
    explicit task_manager(size_t threads = std::thread::hardware_concurrency());
    ~task_manager();

    // blocking copy
    task_manager(const task_manager &) = delete;
    task_manager *operator=(const task_manager &) = delete;

private:
    void worker_loop(std::stop_token token);

    size_t threads_ { 0 };
    std::vector<std::jthread> pool_;
};

} // namespace ivo