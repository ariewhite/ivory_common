add_library(ivory_core STATIC
  src/paths.cpp
)
add_library(ivory::core ALIAS ivory_core)

target_include_directories(ivory_core
  PUBLIC include
)

target_compile_features(ivory_core PUBLIC cxx_std_23)

if(WIN32)
  target_link_libraries(ivory_core PRIVATE shell32 ole32)
endif()