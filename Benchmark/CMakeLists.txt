file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(Benchmark ${SOURCES})

FetchContent_MakeAvailable(benchmark)

target_link_libraries(Benchmark PRIVATE HECS::HECS benchmark::benchmark benchmark::benchmark_main)

