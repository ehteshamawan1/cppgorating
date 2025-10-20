# ============================================================================
# Unit Tests for cppgorating using Google Test
# ============================================================================

message(STATUS "Configuring unit tests...")

# Test source files
set(TEST_SOURCES
    test_main.cpp
    test_database.cpp
    test_http.cpp
    test_curl.cpp
)

# ============================================================================
# Create Test Executable
# ============================================================================

add_executable(cppgorating_tests ${TEST_SOURCES})

# ============================================================================
# Include Directories
# ============================================================================

target_include_directories(cppgorating_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/controllers
    ${CMAKE_SOURCE_DIR}/src/utils
)

# ASIO headers (if available)
if(EXISTS ${CMAKE_SOURCE_DIR}/third_party/asio/include)
    target_include_directories(cppgorating_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/third_party/asio/include
    )
endif()

# MySQL includes (if using custom version)
if(MYSQL_INCLUDE_DIR)
    target_include_directories(cppgorating_tests PRIVATE ${MYSQL_INCLUDE_DIR})
endif()

# ============================================================================
# Link Libraries
# ============================================================================

# Link with Google Test
target_link_libraries(cppgorating_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
)

# Link with Drogon (for HTTP tests)
target_link_libraries(cppgorating_tests PRIVATE Drogon::Drogon)

# Link with libcurl (for HTTP client tests)
target_link_libraries(cppgorating_tests PRIVATE CURL::libcurl)

# Link with Gumbo (for HTML parsing tests)
target_link_libraries(cppgorating_tests PRIVATE unofficial::gumbo::gumbo)

# MySQL Connector
if(MYSQL_CPP_CONN)
    target_link_libraries(cppgorating_tests PRIVATE ${MYSQL_CPP_CONN})
    target_compile_definitions(cppgorating_tests PRIVATE STATIC_CONCPP)
elseif(mysql-connector-cpp_FOUND)
    target_link_libraries(cppgorating_tests PRIVATE mysql-connector-cpp::connector)
endif()

# ============================================================================
# Platform-Specific Libraries
# ============================================================================

if(WIN32)
    # Windows system libraries
    target_link_libraries(cppgorating_tests PRIVATE
        ws2_32
        wsock32
        crypt32
        advapi32
        rpcrt4
        iphlpapi
    )

    # Windows compile definitions
    target_compile_definitions(cppgorating_tests PRIVATE
        _WIN32_WINNT=0x0601
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )

elseif(UNIX)
    # Unix system libraries
    find_package(Threads REQUIRED)
    target_link_libraries(cppgorating_tests PRIVATE
        Threads::Threads
        dl
    )

    # UUID library
    find_library(UUID_LIB uuid)
    if(UUID_LIB)
        target_link_libraries(cppgorating_tests PRIVATE ${UUID_LIB})
    endif()
endif()

# ============================================================================
# Test Configuration
# ============================================================================

# Set C++20 standard
set_target_properties(cppgorating_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# ============================================================================
# Register Tests with CTest
# ============================================================================

# Discover Google Test tests automatically
include(GoogleTest)
gtest_discover_tests(cppgorating_tests
    PROPERTIES
        TIMEOUT 60
)

# Alternative: Add single test target
# add_test(NAME AllTests COMMAND cppgorating_tests)

# ============================================================================
# Installation (Optional)
# ============================================================================

install(TARGETS cppgorating_tests
    RUNTIME DESTINATION bin/tests
)

message(STATUS "Unit tests configured successfully")
message(STATUS "  Test framework: Google Test")
message(STATUS "  Test executable: cppgorating_tests")
message(STATUS "  Run with: ctest or ./bin/cppgorating_tests")
