/// @brief Download from uri / path task

#pragma once

#include <filesystem>

#include <ivory/tasks/task_base.h>

namespace ivo
{

    class download_task : public ivo::base_task
    {
        download_task(const std::string_view path, std::filesystem::);

    private:
        /// @brief Default timeout
        uint64_t timeout {1000};
        /// @brief Default count of retry
        uint64_t retry {3};
    };

} // namespace ivo
