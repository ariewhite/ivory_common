cmake_minimum_required(VERSION 3.30)
project(mc VERSION 0.0.1)

add_executable(${PROJECT_NAME}
  src/test_main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ivory::core
  ivory::tasks
  nlohmann_json::nlohmann_json
  CURL::libcurl
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()