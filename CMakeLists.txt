cmake_minimum_required (VERSION 3.20)

project ("HECS")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(dependencies.cmake)

enable_testing()

add_subdirectory("include/HECS")
add_subdirectory("Test")
add_subdirectory("Benchmark")

option(HECS_BUILD_TESTS         "If tests are built."       OFF)
option(HECS_BUILD_BENCHMARKS    "If benchmarks are built."  OFF)

if(HECS_BUILD_TESTS)
  add_subdirectory("test")
endif()

if(HECS_BUILD_BENCHMARKS)
  add_subdirectory("benchmark")
endif()