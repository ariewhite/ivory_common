add_library(ivory_core INTERFACE)
add_library(ivory::core ALIAS ivory_core)

target_include_directories(ivory_core
  INTERFACE include
)
