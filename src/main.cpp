#ifdef _WIN32
    #define _WIN32_WINNT 0x0601
    #define STATIC_CONCPP
#endif

#include <drogon/drogon.h>
#include "config.hpp"
#include "controllers/MainController.hpp"
#include "controllers/FetchController.hpp"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <iostream>
#include <memory>

// Global MySQL connection
sql::Driver* driver = nullptr;
std::unique_ptr<sql::Connection> con;

// Initialize database connection
bool initDatabase()
{
    try
    {
        // Get MySQL driver instance
        driver = get_driver_instance();

        // Connect to MySQL server
        con.reset(driver->connect(accessPoint.address, accessPoint.user, accessPoint.password));

        // Set database schema
        con->setSchema(accessPoint.database);

        std::cout << "Successfully connected to MySQL database: " << accessPoint.database << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "MySQL Connection Error:" << std::endl;
        std::cerr << "  File: " << __FILE__ << std::endl;
        std::cerr << "  Function: " << __FUNCTION__ << " Line: " << __LINE__ << std::endl;
        std::cerr << "  Error: " << e.what() << std::endl;
        std::cerr << "  MySQL Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "  SQLState: " << e.getSQLState() << std::endl;
        return false;
    }
}

// Get database connection (for use in controllers)
sql::Connection* getDbConnection()
{
    return con.get();
}

int main()
{
    // Initialize database connection
    if (!initDatabase())
    {
        std::cerr << "Failed to initialize database connection. Exiting..." << std::endl;
        return 1;
    }

    // Configure Drogon
    std::cout << "Starting cppgorating server..." << std::endl;
    std::cout << "Listening on http://0.0.0.0:18080" << std::endl;

    // Set number of threads (similar to Crow's multithreaded mode)
    drogon::app()
        .setThreadNum(0)  // 0 = auto-detect optimal thread count
        .addListener("0.0.0.0", 18080);

    // Register controllers
    // Controllers are automatically registered via Drogon's macro system
    // See src/controllers/MainController.hpp for route definitions

    // Run the application
    drogon::app().run();

    return 0;
}
