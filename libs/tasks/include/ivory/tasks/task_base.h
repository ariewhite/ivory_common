/// @brief Base task interface

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>

namespace ivo
{

/// @brief states for task
enum class task_state
{
     pending,
     running,
     succeeded,
     failed,
     cancelled
};

/// @brief error structure
struct task_error
{
     int code = 0;
     std::string msg;
};

/// @brief task progress
struct task_progress
{
     uint64_t done = 0;
     uint64_t total = 0; // 0 - unknown
};

/// @brief interface for tasks
class base_task : public std::enable_shared_from_this<base_task>
{
   public:
     using listener = std::function<void( base_task & )>;

     virtual ~base_task() = default;

     virtual std::string name() const = 0;

     void start();
     void cancel();

     task_state state() const;
     bool is_done() const;
     task_progress progress() const;
     const task_error &error() const;

     void on_progress( listener l );
     void on_finished( listener l );

   protected:
     virtual void on_start() = 0;
     virtual void on_cancel();

     void set_progress( uint64_t done, uint64_t total );

     void succeed();
     void fail( task_error err );

   private:
     bool finish( task_state state, task_error err );

     std::atomic<task_state> state_{ task_state::pending };
     std::atomic<uint64_t> done_{ 0 }, total_{ 0 };
     task_error error_;
     std::vector<listener> progress_listeners_, finished_listeners_;
};

} // namespace ivo