add_library(ivory_tasks_manager STATIC
    src/task_manager.cpp
)

add_library(ivory::tasks_manager ALIAS ivory_tasks_manager)

target_include_directories(ivory_tasks_manager
  PUBLIC include
)
target_link_libraries(ivory_tasks_manager PUBLIC
  ivory::core
  ivory::tasks
)
