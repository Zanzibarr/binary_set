cmake_minimum_required(VERSION 3.14)

project(binary_set_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

add_executable(
  run_tests
  main.cpp
  binary_set_test.cpp
  bs_searcher_test.cpp
)

target_link_libraries(
  run_tests
  GTest::gtest_main
)

target_include_directories(
  run_tests
  PRIVATE
    ${CMAKE_SOURCE_DIR} # To find binary_set.hxx
)

include(GoogleTest)
gtest_discover_tests(run_tests)
