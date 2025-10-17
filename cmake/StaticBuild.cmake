# StaticBuild.cmake
# CMake module for configuring static linking across platforms

message(STATUS "Loading StaticBuild configuration module")

# Force static library building by default
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Platform-specific static linking configuration
if(WIN32)
    # Windows: Use static runtime library
    message(STATUS "  Configuring Windows static runtime")

    # For MSVC, use /MT or /MTd flags
    if(MSVC)
        # This is handled by CMAKE_MSVC_RUNTIME_LIBRARY in root CMakeLists.txt
        # But we can add additional MSVC-specific configurations here

        # Prefer static libraries
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".a" ${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif()

    # For MinGW on Windows
    if(MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    endif()

elseif(UNIX AND NOT APPLE)
    # Linux/FreeBSD: Link standard libraries statically
    message(STATUS "  Configuring Unix static linking")

    # Prefer static libraries when searching
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ${CMAKE_FIND_LIBRARY_SUFFIXES})

    # Static linking flags are set in root CMakeLists.txt
    # Additional options can be added here if needed

    # Option for full static linking (may not work on all systems)
    option(FULL_STATIC_LINKING "Link all libraries statically (including system libs)" OFF)
    if(FULL_STATIC_LINKING)
        message(STATUS "  Full static linking enabled")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif()
endif()

# Force static builds for third-party dependencies
macro(force_static_build)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
    set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static libraries" FORCE)
endmacro()

message(STATUS "  BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "StaticBuild configuration complete")
