cmake_minimum_required(VERSION 3.14)

project(binary_set_benchmarks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googlebenchmark
  URL https://github.com/google/benchmark/archive/v1.8.3.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(
  run_benchmarks
  main_benchmark.cpp
)

target_link_libraries(
  run_benchmarks
  PRIVATE
    benchmark
    benchmark_main
)

target_include_directories(
  run_benchmarks
  PRIVATE
    ${CMAKE_SOURCE_DIR} # To find binary_set.hxx
)