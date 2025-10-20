#include <gtest/gtest.h>
#include <iostream>

/**
 * Main test runner for cppgorating using Google Test
 *
 * This file initializes the Google Test framework and runs all tests.
 * Individual test suites are defined in separate files:
 * - test_database.cpp: MySQL database tests
 * - test_http.cpp: HTTP endpoint tests
 * - test_curl.cpp: libcurl HTTP client tests
 *
 * Usage:
 *   ./cppgorating_tests
 *
 * To run specific test suite:
 *   ./cppgorating_tests --gtest_filter=DatabaseTests.*
 *
 * To list all tests:
 *   ./cppgorating_tests --gtest_list_tests
 */

int main(int argc, char** argv)
{
    std::cout << "================================" << std::endl;
    std::cout << "cppgorating Unit Test Suite" << std::endl;
    std::cout << "Framework: Google Test" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // Run all tests
    int result = RUN_ALL_TESTS();

    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Test run complete" << std::endl;
    std::cout << "================================" << std::endl;

    return result;
}
