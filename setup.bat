@echo off

:: Define variables
set BUILD_DIR=out\build\x64-Debug
set INSTALL_PREFIX=C:\vcpkg\installed\x64-windows

:: Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

:: Configure the project
cmake -S . -B "%BUILD_DIR%" -DCMAKE_INSTALL_PREFIX="%INSTALL_PREFIX%"
if %errorlevel% neq 0 (
    echo Failed to configure CMake project.
    exit /b %errorlevel%
)

:: Install the project (no build step for header-only libraries)
cmake --install "%BUILD_DIR%" --prefix "%INSTALL_PREFIX%"
if %errorlevel% neq 0 (
    echo Install failed.
    exit /b %errorlevel%
)