include(FetchContent)

FetchContent_Declare (
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        GIT_SHALLOW    TRUE
)

FetchContent_Declare (
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.4
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
