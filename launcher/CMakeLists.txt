add_library(ivory_launcher STATIC
  src/version.cpp
  src/manifest.cpp
)
add_library(ivory::laucnher ALIAS ivory_launcher)

message("library ${LIBRARY_OUTPUT_NAME}")

target_include_directories(ivory_launcher
  PUBLIC include
)

target_link_libraries(ivory_launcher PRIVATE
  common
  nlohmann_json::nlohmann_json
  CURL::libcurl
)
