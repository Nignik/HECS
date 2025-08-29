add_library(HECS INTERFACE)
add_library(hecs::hecs ALIAS HECS)

target_include_directories(HECS INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})