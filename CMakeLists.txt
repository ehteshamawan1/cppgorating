cmake_minimum_required(VERSION 3.20)
project(cppgorating VERSION 1.0.0 LANGUAGES CXX)

# C++20 standard required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Platform detection and configuration
message(STATUS "Building for platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Static linking configuration
if(WIN32)
    message(STATUS "Configuring for Windows with static runtime")
    # Use static runtime library on Windows
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Add Windows-specific definitions
    add_compile_definitions(
        _WIN32_WINNT=0x0601  # Windows 7 minimum
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        STATIC_CONCPP  # Static MySQL Connector/C++
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
    message(STATUS "Configuring for Unix-like system (Linux/FreeBSD) with static linking")

    # Static linking flags for GCC/Clang
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

    # Additional flags for complete static linking (optional, can be enabled if needed)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

    add_compile_options(
        -Wall
        -Wextra
        -pedantic
    )

    # Enable PIC (Position Independent Code) for libraries
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    message(STATUS "Build type not specified, defaulting to Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Enable testing
enable_testing()

# Include CMake utility modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cmake/StaticBuild.cmake)

# Add subdirectories
add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(tests)

# Print configuration summary
message(STATUS "")
message(STATUS "=== cppgorating Configuration Summary ===")
message(STATUS "  Project version: ${PROJECT_VERSION}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "=========================================")
message(STATUS "")
