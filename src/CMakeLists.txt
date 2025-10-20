# ============================================================================
# Source Files
# ============================================================================

set(SOURCES
    main.cpp
)

set(HEADERS
    config.hpp
    AccessPoint.hpp
    controllers/MainController.hpp
    controllers/FetchController.hpp
    utils/HtmlParser.hpp
)

# ============================================================================
# Create Executable
# ============================================================================

add_executable(cppgorating ${SOURCES} ${HEADERS})

# ============================================================================
# Include Directories
# ============================================================================

target_include_directories(cppgorating PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# ASIO is header-only, add if exists
if(EXISTS ${CMAKE_SOURCE_DIR}/third_party/asio/include)
    target_include_directories(cppgorating PRIVATE
        ${CMAKE_SOURCE_DIR}/third_party/asio/include
    )
    message(STATUS "Using ASIO from third_party/asio")
endif()

# MySQL Connector include directory (if using custom version)
if(MYSQL_INCLUDE_DIR)
    target_include_directories(cppgorating PRIVATE ${MYSQL_INCLUDE_DIR})
    message(STATUS "Using MySQL includes from: ${MYSQL_INCLUDE_DIR}")
endif()

# ============================================================================
# Link Libraries (vcpkg-managed)
# ============================================================================

# Link with Drogon (automatically includes Trantor, jsoncpp, etc.)
target_link_libraries(cppgorating PRIVATE Drogon::Drogon)

# Link with libcurl
target_link_libraries(cppgorating PRIVATE CURL::libcurl)

# Link with Gumbo HTML parser
target_link_libraries(cppgorating PRIVATE unofficial::gumbo::gumbo)

# ============================================================================
# MySQL Connector Linking
# ============================================================================

if(MYSQL_CPP_CONN)
    # Using custom MySQL Connector from third_party
    target_link_libraries(cppgorating PRIVATE ${MYSQL_CPP_CONN})
    message(STATUS "Linking with custom MySQL Connector: ${MYSQL_CPP_CONN}")

    # Add STATIC_CONCPP definition for static linking
    target_compile_definitions(cppgorating PRIVATE STATIC_CONCPP)

elseif(mysql-connector-cpp_FOUND)
    # Using vcpkg MySQL Connector
    target_link_libraries(cppgorating PRIVATE mysql-connector-cpp::connector)
    message(STATUS "Linking with vcpkg MySQL Connector")
else()
    message(WARNING "MySQL Connector not linked - database functionality will not work")
endif()

# ============================================================================
# Platform-Specific Libraries
# ============================================================================

if(WIN32)
    # Windows system libraries
    target_link_libraries(cppgorating PRIVATE
        ws2_32          # Winsock 2
        wsock32         # Winsock
        crypt32         # Cryptography API
        advapi32        # Advanced Windows API
        rpcrt4          # RPC Runtime
        iphlpapi        # IP Helper API
    )

    message(STATUS "Linked Windows system libraries")

elseif(UNIX)
    # Unix system libraries
    find_package(Threads REQUIRED)
    target_link_libraries(cppgorating PRIVATE
        Threads::Threads
        dl              # Dynamic linking loader
    )

    # UUID library (if available, sometimes needed by Drogon)
    find_library(UUID_LIB uuid)
    if(UUID_LIB)
        target_link_libraries(cppgorating PRIVATE ${UUID_LIB})
        message(STATUS "Linked UUID library")
    endif()

    message(STATUS "Linked Unix system libraries")
endif()

# ============================================================================
# Compiler Optimizations
# ============================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(cppgorating PRIVATE /O2)
    else()
        target_compile_options(cppgorating PRIVATE -O3)
    endif()
endif()

# ============================================================================
# Installation
# ============================================================================

install(TARGETS cppgorating
    RUNTIME DESTINATION bin
)

message(STATUS "cppgorating executable configured successfully")
