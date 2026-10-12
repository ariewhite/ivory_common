add_library(ivory STATIC
  src/version.cpp
  src/manifest.cpp
)

target_include_directories(ivory
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ivory PRIVATE
  common
  nlohmann_json::nlohmann_json
  CURL::libcurl
)
