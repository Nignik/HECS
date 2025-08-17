add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>  # During the build
    $<INSTALL_INTERFACE:include>  # After installation - changed from include/HECS
)

# Install the header files to the "include/HECS" directory in the install location
install(DIRECTORY Core/ DESTINATION include/HECS
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install the CMake configuration for package finding
install(TARGETS ${PROJECT_NAME} EXPORT HECSConfig)

# Generate and install the CMake config file for find_package
install(EXPORT HECSConfig
    FILE HECSConfig.cmake
    NAMESPACE hecs::  # Changed from HECS:: to hecs::
    DESTINATION lib/cmake/HECS
)