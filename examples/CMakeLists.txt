cmake_minimum_required(VERSION 3.10)

project(binary_set_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(basic_usage basic_usage.cpp)
target_include_directories(basic_usage PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(iteration iteration.cpp)
target_include_directories(iteration PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(set_operations set_operations.cpp)
target_include_directories(set_operations PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(subset_search subset_search.cpp)
target_include_directories(subset_search PRIVATE ${CMAKE_SOURCE_DIR})
