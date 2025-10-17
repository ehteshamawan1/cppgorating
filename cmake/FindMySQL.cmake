# FindMySQL.cmake
# CMake find module for MySQL Connector/C++

# This module defines:
#  MySQL_FOUND - System has MySQL Connector/C++
#  MySQL_INCLUDE_DIRS - MySQL include directories
#  MySQL_LIBRARIES - Libraries needed to use MySQL
#  MySQL_VERSION - MySQL Connector version

message(STATUS "Searching for MySQL Connector/C++")

# Search in common locations and our third_party directory
set(MySQL_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/third_party/mysql
    ${CMAKE_SOURCE_DIR}/lib/mysql
    /usr/local/mysql
    /usr/mysql
    /opt/mysql
    $ENV{MYSQL_DIR}
    $ENV{MySQL_DIR}
)

# Find include directory
find_path(MySQL_INCLUDE_DIR
    NAMES mysql_driver.h mysql_connection.h
    PATHS ${MySQL_SEARCH_PATHS}
    PATH_SUFFIXES include include/jdbc jdbc
    DOC "MySQL Connector/C++ include directory"
)

# Platform-specific library names
if(WIN32)
    set(MySQL_LIB_NAMES mysqlcppconn-static mysqlcppconn mysqlcppconn8-static mysqlcppconn8)
    set(MySQL_LIB_SUFFIXES lib lib64/win/debug lib64/win/release lib/win/debug lib/win/release)
else()
    set(MySQL_LIB_NAMES mysqlcppconn-static mysqlcppconn mysqlcppconn8-static mysqlcppconn8)
    set(MySQL_LIB_SUFFIXES lib lib64 lib/linux lib64/linux)
endif()

# Find library
find_library(MySQL_LIBRARY
    NAMES ${MySQL_LIB_NAMES}
    PATHS ${MySQL_SEARCH_PATHS}
    PATH_SUFFIXES ${MySQL_LIB_SUFFIXES}
    DOC "MySQL Connector/C++ library"
)

# Handle version (if available)
if(MySQL_INCLUDE_DIR)
    set(MySQL_VERSION "8.0.0")  # Default version, can be detected from headers if needed
endif()

# Handle standard arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQL
    REQUIRED_VARS MySQL_LIBRARY MySQL_INCLUDE_DIR
    VERSION_VAR MySQL_VERSION
)

# Set output variables
if(MySQL_FOUND)
    set(MySQL_LIBRARIES ${MySQL_LIBRARY})
    set(MySQL_INCLUDE_DIRS ${MySQL_INCLUDE_DIR})

    message(STATUS "  MySQL include dir: ${MySQL_INCLUDE_DIRS}")
    message(STATUS "  MySQL library: ${MySQL_LIBRARIES}")

    # Create imported target
    if(NOT TARGET MySQL::MySQL)
        add_library(MySQL::MySQL UNKNOWN IMPORTED)
        set_target_properties(MySQL::MySQL PROPERTIES
            IMPORTED_LOCATION "${MySQL_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${MySQL_INCLUDE_DIR}"
        )

        # Add required definitions for static linking
        if(WIN32)
            set_target_properties(MySQL::MySQL PROPERTIES
                INTERFACE_COMPILE_DEFINITIONS "STATIC_CONCPP"
            )
        endif()
    endif()
endif()

mark_as_advanced(MySQL_INCLUDE_DIR MySQL_LIBRARY)
