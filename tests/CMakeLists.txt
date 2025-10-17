# Unit tests for cppgorating using Unittest++

message(STATUS "Configuring unit tests...")

# Check if Unittest++ is available
if(NOT TARGET UnitTest++)
    message(WARNING "Unittest++ not found - tests will not be built")
    message(WARNING "Clone Unittest++ into third_party/unittest-cpp to enable testing")
    return()
endif()

# Test source files
set(TEST_SOURCES
    test_main.cpp
    test_database.cpp
    test_http.cpp
    test_curl.cpp
)

# Create test executable
add_executable(cppgorating_tests ${TEST_SOURCES})

# Include directories
target_include_directories(cppgorating_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src                           # For accessing main app headers
    ${CMAKE_SOURCE_DIR}/src/controllers               # Controller headers
    ${CMAKE_SOURCE_DIR}/src/utils                     # Utility headers
    ${CMAKE_SOURCE_DIR}/third_party/asio/include      # ASIO headers
    ${CMAKE_SOURCE_DIR}/third_party/mysql/include/jdbc # MySQL headers
)

# Link with Unittest++
target_link_libraries(cppgorating_tests PRIVATE UnitTest++)

# Link with libcurl for HTTP tests
if(TARGET libcurl)
    target_link_libraries(cppgorating_tests PRIVATE libcurl)
elseif(TARGET CURL::libcurl)
    target_link_libraries(cppgorating_tests PRIVATE CURL::libcurl)
else()
    message(WARNING "libcurl not found - curl tests may not compile")
endif()

# Platform-specific configuration
if(WIN32)
    message(STATUS "Configuring Windows-specific settings for tests")

    # MySQL Connector/C++ (same as main app)
    set(MYSQL_BASE ${CMAKE_SOURCE_DIR}/third_party/mysql/lib64/win/debug)
    if(EXISTS ${MYSQL_BASE}/mysqlcppconn-static.lib)
        target_link_libraries(cppgorating_tests PRIVATE
            ${MYSQL_BASE}/mysqlcppconn-static.lib
            ${MYSQL_BASE}/libssl.lib
            ${MYSQL_BASE}/libcrypto.lib
        )
    endif()

    # Windows system libraries
    target_link_libraries(cppgorating_tests PRIVATE
        ws2_32
        wsock32
        crypt32
        advapi32
    )

    # Windows compile definitions
    target_compile_definitions(cppgorating_tests PRIVATE
        _WIN32_WINNT=0x0601
        STATIC_CONCPP
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )

elseif(UNIX)
    message(STATUS "Configuring Unix-specific settings for tests")

    # MySQL Connector/C++
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
        target_link_libraries(cppgorating_tests PRIVATE ${MYSQLCPPCONN_LIB})
    endif()

    # OpenSSL
    if(NOT TARGET OpenSSL::SSL)
        find_package(OpenSSL REQUIRED)
    endif()
    target_link_libraries(cppgorating_tests PRIVATE OpenSSL::SSL OpenSSL::Crypto)

    # Threading
    find_package(Threads REQUIRED)
    target_link_libraries(cppgorating_tests PRIVATE Threads::Threads)

    # System libraries
    target_link_libraries(cppgorating_tests PRIVATE dl)
endif()

# Set C++20 standard
set_target_properties(cppgorating_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Enable testing
enable_testing()

# Add test to CTest
add_test(NAME AllTests COMMAND cppgorating_tests)

# Additional test configurations
set_tests_properties(AllTests PROPERTIES
    TIMEOUT 60  # 60 second timeout for all tests
)

# Installation (optional)
install(TARGETS cppgorating_tests
    RUNTIME DESTINATION bin/tests
)

message(STATUS "Unit tests configured successfully")
message(STATUS "  Test executable: cppgorating_tests")
message(STATUS "  Run with: ctest or ./bin/cppgorating_tests")
