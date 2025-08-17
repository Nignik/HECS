file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(EcsTest ${SOURCES})
target_link_libraries(EcsTest PRIVATE HECS GTest::gtest GTest::gtest_main)
add_test(EcsTest EcsTest)