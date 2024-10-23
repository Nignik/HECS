add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>  # During the build
    $<INSTALL_INTERFACE:include/Core>  # After installation
)

# Install the header files to the "include" directory in the install location
install(DIRECTORY Core/ DESTINATION include/Core)

# Install the CMake configuration for package finding
install(TARGETS HECS EXPORT HECSConfig)

# Generate and install the CMake config file for find_package
install(EXPORT HECSConfig
    FILE HECSConfig.cmake
    NAMESPACE HECS::
    DESTINATION lib/cmake/HECS
)


