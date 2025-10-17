# Source files for cppgorating executable
set(SOURCES
    main.cpp
    # Add more .cpp files here as needed
)

# Header files (for IDE organization)
set(HEADERS
    config.hpp
    AccessPoint.hpp
    controllers/MainController.hpp
    controllers/FetchController.hpp
    utils/HtmlParser.hpp
)

# Create executable
add_executable(cppgorating ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(cppgorating PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}                           # For config.hpp, AccessPoint.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers               # Controller headers
    ${CMAKE_CURRENT_SOURCE_DIR}/utils                     # Utility headers
    ${CMAKE_SOURCE_DIR}/third_party/asio/include          # ASIO headers
    ${CMAKE_SOURCE_DIR}/third_party/mysql/include/jdbc    # MySQL Connector headers
)

# Link with Drogon framework (if available)
if(TARGET drogon)
    message(STATUS "Linking with Drogon framework")
    target_link_libraries(cppgorating PRIVATE drogon)
else()
    message(WARNING "Drogon target not found - make sure to clone Drogon into third_party/")
endif()

# Link with libcurl (if available)
if(TARGET libcurl)
    message(STATUS "Linking with libcurl")
    target_link_libraries(cppgorating PRIVATE libcurl)
elseif(TARGET CURL::libcurl)
    target_link_libraries(cppgorating PRIVATE CURL::libcurl)
else()
    message(WARNING "libcurl not found - /fetch endpoint may not work")
endif()

# Link with Gumbo parser (if available)
if(TARGET gumbo)
    message(STATUS "Linking with Gumbo HTML parser")
    target_link_libraries(cppgorating PRIVATE gumbo)
else()
    message(WARNING "Gumbo parser not found - HTML parsing may not work")
endif()

# Platform-specific configuration
if(WIN32)
    message(STATUS "Configuring Windows-specific settings for cppgorating")

    # MySQL Connector/C++ (static linking)
    find_package(MySQL QUIET)
    if(MySQL_FOUND)
        target_link_libraries(cppgorating PRIVATE MySQL::MySQL)
    else()
        # Fallback: Direct linking with MySQL libraries
        set(MYSQL_BASE ${CMAKE_SOURCE_DIR}/third_party/mysql/lib64/win/debug)

        if(EXISTS ${MYSQL_BASE}/mysqlcppconn-static.lib)
            target_link_libraries(cppgorating PRIVATE
                ${MYSQL_BASE}/mysqlcppconn-static.lib
                ${MYSQL_BASE}/libssl.lib
                ${MYSQL_BASE}/libcrypto.lib
            )
            message(STATUS "Linked with MySQL Connector/C++ (static) from third_party/")
        else()
            message(WARNING "MySQL Connector libraries not found in ${MYSQL_BASE}")
        endif()
    endif()

    # Windows system libraries
    target_link_libraries(cppgorating PRIVATE
        ws2_32          # Winsock 2
        wsock32         # Winsock
        crypt32         # Cryptography API
        advapi32        # Advanced Windows API
        rpcrt4          # RPC Runtime
        iphlpapi        # IP Helper API (sometimes needed by networking libs)
    )

    # Windows compile definitions
    target_compile_definitions(cppgorating PRIVATE
        _WIN32_WINNT=0x0601       # Windows 7+
        STATIC_CONCPP             # Static MySQL Connector
        NOMINMAX                  # Prevent Windows.h min/max macros
        WIN32_LEAN_AND_MEAN       # Reduce Windows.h bloat
    )

elseif(UNIX)
    message(STATUS "Configuring Unix-specific settings for cppgorating")

    # MySQL Connector/C++
    find_package(MySQL QUIET)
    if(MySQL_FOUND)
        target_link_libraries(cppgorating PRIVATE MySQL::MySQL)
        message(STATUS "Linked with MySQL Connector/C++ via FindMySQL")
    else()
        # Try to find in third_party or system
        find_library(MYSQLCPPCONN_LIB
            NAMES mysqlcppconn-static mysqlcppconn
            PATHS
                ${CMAKE_SOURCE_DIR}/third_party/mysql/lib64
                ${CMAKE_SOURCE_DIR}/third_party/mysql/lib
                /usr/lib /usr/local/lib
                /usr/lib64 /usr/local/lib64
                /usr/lib/x86_64-linux-gnu
        )

        if(MYSQLCPPCONN_LIB)
            target_link_libraries(cppgorating PRIVATE ${MYSQLCPPCONN_LIB})
            message(STATUS "Linked with MySQL Connector: ${MYSQLCPPCONN_LIB}")

            # Add MySQL include directory
            target_include_directories(cppgorating PRIVATE
                ${CMAKE_SOURCE_DIR}/third_party/mysql/include/jdbc
            )
        else()
            message(FATAL_ERROR "MySQL Connector/C++ not found. Install libmysqlcppconn-dev or add to third_party/mysql")
        endif()
    endif()

    # OpenSSL (should already be found by third_party/CMakeLists.txt)
    if(NOT TARGET OpenSSL::SSL)
        find_package(OpenSSL REQUIRED)
    endif()
    target_link_libraries(cppgorating PRIVATE OpenSSL::SSL OpenSSL::Crypto)

    # Threading library
    find_package(Threads REQUIRED)
    target_link_libraries(cppgorating PRIVATE Threads::Threads)

    # System libraries
    target_link_libraries(cppgorating PRIVATE
        dl              # Dynamic linking
    )

    # Optional: UUID library (sometimes needed by Drogon)
    find_library(UUID_LIB uuid)
    if(UUID_LIB)
        target_link_libraries(cppgorating PRIVATE ${UUID_LIB})
    endif()

    # Optional: zlib (for compression support)
    find_package(ZLIB)
    if(ZLIB_FOUND)
        target_link_libraries(cppgorating PRIVATE ZLIB::ZLIB)
    endif()
endif()

# Set C++20 standard
set_target_properties(cppgorating PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Optimization flags for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(cppgorating PRIVATE /O2)
    else()
        target_compile_options(cppgorating PRIVATE -O3)
    endif()
endif()

# Installation (optional)
install(TARGETS cppgorating
    RUNTIME DESTINATION bin
)

message(STATUS "cppgorating executable configured successfully")
