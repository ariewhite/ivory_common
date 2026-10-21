cmake_minimum_required(VERSION 3.30)
project(mc VERSION 0.0.1)

add_executable(${PROJECT_NAME}
  src/test_main.cpp
  src/test_download_task.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ivory::core
  ivory::tasks
  ivory::launcher
  nlohmann_json::nlohmann_json
  OpenSSL::SSL
  CURL::libcurl
  GTest::gtest_main
)

# if(BUILD_TESTING)
  # add_subdirectory(tests)
# endif()