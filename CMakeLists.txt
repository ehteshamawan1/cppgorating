cmake_minimum_required(VERSION 3.20)
project(cppgorating VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# Project Configuration
# ============================================================================

# C++20 standard required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# ============================================================================
# Platform-Specific Configuration
# ============================================================================

message(STATUS "Building for platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(WIN32)
    message(STATUS "Configuring for Windows with static runtime")

    # Use static runtime library on Windows (when using x64-windows-static triplet)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Windows-specific definitions
    add_compile_definitions(
        _WIN32_WINNT=0x0601  # Windows 7 minimum
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )

    # MSVC-specific flags
    if(MSVC)
        add_compile_options(
            /W4           # Warning level 4
            /permissive-  # Conformance mode
            /Zc:__cplusplus  # Enable __cplusplus macro
        )
    endif()

elseif(UNIX)
    message(STATUS "Configuring for Unix-like system (Linux/FreeBSD)")

    # Static linking flags for GCC/Clang
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

    add_compile_options(
        -Wall
        -Wextra
        -pedantic
    )

    # Enable PIC (Position Independent Code) for libraries
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# ============================================================================
# Dependencies (via vcpkg)
# ============================================================================

# Find all required packages (managed by vcpkg)
find_package(Drogon CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(unofficial-gumbo CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# MySQL Connector - try to find it
# Note: If MySQL Connector is not in vcpkg or you use a custom version,
# it can still be found in third_party/mysql or system paths
find_library(MYSQL_CPP_CONN
    NAMES mysqlcppconn mysqlcppconn-static
    PATHS
        ${CMAKE_SOURCE_DIR}/third_party/mysql/lib64/win/debug
        ${CMAKE_SOURCE_DIR}/third_party/mysql/lib64
        ${CMAKE_SOURCE_DIR}/third_party/mysql/lib
    NO_DEFAULT_PATH
)

if(MYSQL_CPP_CONN)
    message(STATUS "Found MySQL Connector: ${MYSQL_CPP_CONN}")
    set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/mysql/include/jdbc)
else()
    # Try to use vcpkg version or system version
    find_package(unofficial-mysql-connector-cpp CONFIG)
    if(unofficial-mysql-connector-cpp_FOUND)
        message(STATUS "Found MySQL Connector via vcpkg")
        set(MYSQL_CPP_CONN unofficial::mysql-connector-cpp::connector)
    else()
        message(WARNING "MySQL Connector not found - database functionality may not work")
    endif()
endif()

# ============================================================================
# Enable Testing
# ============================================================================
enable_testing()

# ============================================================================
# Add Subdirectories
# ============================================================================
add_subdirectory(src)
add_subdirectory(tests)

# ============================================================================
# Configuration Summary
# ============================================================================
message(STATUS "")
message(STATUS "=== cppgorating Configuration Summary ===")
message(STATUS "  Project version: ${PROJECT_VERSION}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Drogon: Found")
message(STATUS "  CURL: Found")
message(STATUS "  Gumbo: Found")
message(STATUS "  GTest: Found")
if(MYSQL_CPP_CONN OR mysql-connector-cpp_FOUND)
    message(STATUS "  MySQL Connector: Found")
else()
    message(STATUS "  MySQL Connector: NOT FOUND")
endif()
message(STATUS "=========================================")
message(STATUS "")
