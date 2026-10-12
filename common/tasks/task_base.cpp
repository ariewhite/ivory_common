#include "tasks/task_base.h"
#include <exception>

namespace ivo
{

void base_task::start()
{
     task_state expected = task_state::pending;
     if ( !state_.compare_exchange_strong( expected, task_state::running ) )
          return;

     try
     {
          on_start();
     }
     catch ( const std::exception &e )
     {
          fail( { -1, e.what() } );
     }
}

void base_task::cancel()
{
     if ( finish( task_state::cancelled, {} ) )
          on_cancel();
}

task_state base_task::state() const { state_.load(); }

bool base_task::is_done() const
{
     auto s = state();
     return s == task_state::succeeded || s == task_state::failed || s == task_state::cancelled;
}

task_progress base_task::progress() const { return { done_.load(), total_.load() }; }

const task_error &base_task::error() const { return error_; }

void base_task::on_progress( listener l ) { progress_listeners_.push_back( std::move( l ) ); }

void base_task::on_finished( listener l ) { finished_listeners_.push_back( std::move( l ) ); }

void base_task::set_progress( uint64_t done, uint64_t total )
{
     done_ = done;
     total_ = total;
     for ( auto &l : progress_listeners_ )
     {
          l( *this );
     }
}

void base_task::succeed() { finish( task_state::succeeded, {} ); }

void base_task::fail( task_error e ) { finish( task_state::failed, { std::move( e ) } ); }

bool base_task::finish( task_state state, task_error err )
{
     task_state expected = task_state::running;
     if ( !state_.compare_exchange_strong( expected, state ) )
          return false;

     error_ = std::move( err );
     for ( auto &l : finished_listeners_ )
          l( *this );
     return true;
}

} // namespace ivo