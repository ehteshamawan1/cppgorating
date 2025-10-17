#include <UnitTest++/UnitTest++.h>
#include <iostream>

/**
 * Main test runner for cppgorating
 *
 * This file initializes the Unittest++ framework and runs all tests.
 * Individual test suites are defined in separate files:
 * - test_database.cpp: MySQL database tests
 * - test_http.cpp: HTTP endpoint tests
 * - test_curl.cpp: libcurl HTTP client tests
 *
 * Usage:
 *   ./cppgorating_tests
 *
 * To run specific test suite:
 *   ./cppgorating_tests -suite DatabaseTests
 */

int main(int argc, char** argv)
{
    std::cout << "================================" << std::endl;
    std::cout << "cppgorating Unit Test Suite" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    // Run all tests
    int result = UnitTest::RunAllTests();

    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Test run complete" << std::endl;
    std::cout << "================================" << std::endl;

    return result;
}
