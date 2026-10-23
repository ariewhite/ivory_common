cmake_minimum_required(VERSION 3.30)
project(ivory_tests VERSION 0.0.1)

# gtest
add_executable(ivory_tests
  src/test_download_task.cpp
)

target_link_libraries(ivory_tests PRIVATE
  ivory::core
  ivory::tasks
  ivory::launcher
  ivory::net
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ivory_tests DISCOVERY_MODE PRE_TEST)

add_executable(ivorym src/test_main.cpp)
target_link_libraries(ivorym PRIVATE
  ivory::core
  ivory::net
  OpenSSL::Crypto
  CURL::libcurl
)