# Third-party dependencies for cppgorating
# All dependencies are configured for static linking

message(STATUS "=======================================================")
message(STATUS "Configuring third-party dependencies...")
message(STATUS "")
message(STATUS "NOTE: If you get errors about missing dependencies,")
message(STATUS "please follow the setup instructions in:")
message(STATUS "  - third_party/README.md")
message(STATUS "  - third_party/SETUP_INSTRUCTIONS.txt")
message(STATUS "")
message(STATUS "Quick setup: Clone required dependencies into third_party/")
message(STATUS "See README.md section 'Quick Start - Step 2' for details.")
message(STATUS "=======================================================")
message(STATUS "")

# Force static library builds
include(${CMAKE_SOURCE_DIR}/cmake/StaticBuild.cmake)
force_static_build()

#=============================================================================
# OpenSSL Configuration
#=============================================================================
message(STATUS "Searching for OpenSSL...")
find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    message(STATUS "  OpenSSL version: ${OPENSSL_VERSION}")
    message(STATUS "  OpenSSL include: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "  OpenSSL libraries: ${OPENSSL_LIBRARIES}")
endif()

#=============================================================================
# jsoncpp (Required by Drogon)
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/CMakeLists.txt)
    message(STATUS "Adding jsoncpp...")
    set(JSONCPP_WITH_TESTS OFF CACHE BOOL "" FORCE)
    set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "" FORCE)
    set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
    add_subdirectory(jsoncpp EXCLUDE_FROM_ALL)
else()
    message(FATAL_ERROR "jsoncpp not found in third_party/jsoncpp!\n"
            "This is a REQUIRED dependency. Please clone it:\n"
            "  cd third_party\n"
            "  git clone --depth 1 https://github.com/open-source-parsers/jsoncpp.git\n"
            "See third_party/README.md for complete setup instructions.")
endif()

#=============================================================================
# Trantor (Network library required by Drogon)
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/trantor/CMakeLists.txt)
    message(STATUS "Adding Trantor...")
    set(BUILD_TRANTOR_SHARED OFF CACHE BOOL "" FORCE)
    add_subdirectory(trantor EXCLUDE_FROM_ALL)
else()
    message(FATAL_ERROR "Trantor not found in third_party/trantor!\n"
            "This is a REQUIRED dependency. Please clone it:\n"
            "  cd third_party\n"
            "  git clone --depth 1 https://github.com/an-tao/trantor.git\n"
            "See third_party/README.md for complete setup instructions.")
endif()

#=============================================================================
# Drogon Web Framework
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/drogon/CMakeLists.txt)
    message(STATUS "Adding Drogon web framework...")

    # Drogon build options
    set(BUILD_DROGON_SHARED OFF CACHE BOOL "Build Drogon as shared library" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "Build Drogon examples" FORCE)
    set(BUILD_CTL ON CACHE BOOL "Build drogon_ctl command line tool" FORCE)
    set(BUILD_ORM OFF CACHE BOOL "Build Drogon ORM" FORCE)

    # Disable testing
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

    add_subdirectory(drogon EXCLUDE_FROM_ALL)
    message(STATUS "  Drogon configured for static linking")
else()
    message(FATAL_ERROR "Drogon not found in third_party/drogon!\n"
            "This is a REQUIRED dependency. Please clone it:\n"
            "  cd third_party\n"
            "  git clone --depth 1 https://github.com/drogonframework/drogon.git\n"
            "See third_party/README.md for complete setup instructions.")
endif()

#=============================================================================
# libcurl (HTTP Client Library)
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libcurl/CMakeLists.txt)
    message(STATUS "Adding libcurl...")

    # libcurl build options for static linking
    set(BUILD_CURL_EXE OFF CACHE BOOL "Build curl executable" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
    set(CURL_STATICLIB ON CACHE BOOL "Build static library" FORCE)
    set(CMAKE_USE_OPENSSL ON CACHE BOOL "Use OpenSSL" FORCE)
    set(HTTP_ONLY OFF CACHE BOOL "Support only HTTP" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "Build tests" FORCE)

    add_subdirectory(libcurl EXCLUDE_FROM_ALL)
    message(STATUS "  libcurl configured for static linking")
else()
    message(FATAL_ERROR "libcurl not found in third_party/libcurl!\n"
            "This is a REQUIRED dependency. Please clone it:\n"
            "  cd third_party\n"
            "  git clone --depth 1 https://github.com/curl/curl.git libcurl\n"
            "See third_party/README.md for complete setup instructions.")
endif()

#=============================================================================
# Unittest++ (Unit Testing Framework)
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/unittest-cpp/CMakeLists.txt)
    message(STATUS "Adding Unittest++...")

    set(UTPP_INCLUDE_TESTS_IN_BUILD OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    add_subdirectory(unittest-cpp EXCLUDE_FROM_ALL)
    message(STATUS "  Unittest++ configured")
else()
    message(WARNING "Unittest++ not found in third_party/unittest-cpp - clone it first!")
endif()

#=============================================================================
# Gumbo HTML Parser
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/gumbo-parser/CMakeLists.txt)
    message(STATUS "Adding Gumbo HTML parser...")

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    add_subdirectory(gumbo-parser EXCLUDE_FROM_ALL)
    message(STATUS "  Gumbo parser configured")
else()
    message(WARNING "Gumbo parser not found in third_party/gumbo-parser - clone it first!")
endif()

#=============================================================================
# ASIO (Header-only - already included)
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/asio/include)
    message(STATUS "ASIO headers found: ${CMAKE_CURRENT_SOURCE_DIR}/asio/include")
    # ASIO is header-only, just needs to be in include path
else()
    message(FATAL_ERROR "ASIO not found in third_party/asio!")
endif()

#=============================================================================
# MySQL Connector/C++ (Pre-built)
#=============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/mysql/include)
    message(STATUS "MySQL Connector/C++ found: ${CMAKE_CURRENT_SOURCE_DIR}/mysql")
    # MySQL is pre-built, will be linked in src/CMakeLists.txt
else()
    message(FATAL_ERROR "MySQL Connector not found in third_party/mysql!")
endif()

#=============================================================================
# Summary
#=============================================================================
message(STATUS "")
message(STATUS "=== Third-Party Dependencies Summary ===")
message(STATUS "  OpenSSL: ${OpenSSL_FOUND}")
message(STATUS "  jsoncpp: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/CMakeLists.txt}")
message(STATUS "  Trantor: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/trantor/CMakeLists.txt}")
message(STATUS "  Drogon: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/drogon/CMakeLists.txt}")
message(STATUS "  libcurl: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libcurl/CMakeLists.txt}")
message(STATUS "  Unittest++: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/unittest-cpp/CMakeLists.txt}")
message(STATUS "  Gumbo: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/gumbo-parser/CMakeLists.txt}")
message(STATUS "  ASIO: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/asio/include}")
message(STATUS "  MySQL: ${EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/mysql/include}")
message(STATUS "=========================================")
message(STATUS "")
