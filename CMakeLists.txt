cmake_minimum_required(VERSION 3.30)
project(ivory VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
option(BUILD_TESTING "Build tests" ON)

include(FetchContent)

# nlohmann
find_package(nlohmann_json REQUIRED)

# libcurl
find_package(CURL REQUIRED)

# openssl
find_package(OpenSSL REQUIRED)

# tests
if(BUILD_TESTING)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(libs/net)
add_subdirectory(libs/core)
add_subdirectory(libs/tasks)
add_subdirectory(launcher)
add_subdirectory(tests)