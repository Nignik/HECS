file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
enable_testing()

find_package(HECS REQUIRED)
add_executable(ComponentTest ${SOURCES})
target_link_libraries(ComponentTest PRIVATE HECS::HECS GTest::gtest GTest::gtest_main)
add_test(ComponentTest ComponentTest)


