file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(HECS REQUIRED)

add_executable(ComponentTest ${SOURCES})

target_link_libraries(ComponentTest PRIVATE HECS::HECS GTest::gtest GTest::gtest_main)
add_test(ComponentTest ComponentTest)


