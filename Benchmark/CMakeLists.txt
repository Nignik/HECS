file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

FetchContent_MakeAvailable(benchmark)
FetchContent_MakeAvailable(glm)

add_executable(Benchmark ${SOURCES})
target_link_libraries(Benchmark PRIVATE HECS benchmark::benchmark benchmark::benchmark_main glm::glm)