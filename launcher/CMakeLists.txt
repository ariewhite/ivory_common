add_library(ivory STATIC
  src/version.cpp
)

target_include_directories(ivory
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_directories(ivoryc PRIVATE
  common
  nlohmann_json::nlohmann_json
)
