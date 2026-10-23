add_library(ivory_tasks STATIC
  src/task_base.cpp
  src/download_task.cpp
)
add_library(ivory::tasks ALIAS ivory_tasks)

target_include_directories(ivory_tasks
  PUBLIC include
)
target_link_libraries(ivory_tasks PUBLIC
  ivory::core
  ivory::net
)