cmake_minimum_required(VERSION 3.10)

project(binary_set)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

enable_testing()

include(FetchContent) # Include FetchContent early

add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(benchmarks)
