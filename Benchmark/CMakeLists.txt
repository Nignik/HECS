file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(Benchmark ${SOURCES})

find_package(HECS REQUIRED)
find_package(benchmark CONFIG REQUIRED)

target_link_libraries(Benchmark PRIVATE HECS::HECS benchmark::benchmark benchmark::benchmark_main)

