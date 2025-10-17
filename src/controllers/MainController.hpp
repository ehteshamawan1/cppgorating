#pragma once

#include <drogon/HttpController.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

// Forward declaration of getDbConnection from main.cpp
extern sql::Connection* getDbConnection();

using namespace drogon;

/**
 * MainController - Handles main route requests
 *
 * This controller implements the same functionality as the original Crow application:
 * - GET / - Returns the count of countries from the database
 */
class MainController : public drogon::HttpController<MainController>
{
public:
    METHOD_LIST_BEGIN
    // Register GET handler for "/"
    ADD_METHOD_TO(MainController::index, "/", Get);
    METHOD_LIST_END

    /**
     * GET / handler
     * Returns the count of countries from the database
     */
    void index(const HttpRequestPtr& req,
               std::function<void(const HttpResponsePtr&)>&& callback)
    {
        try
        {
            sql::Connection* dbConn = getDbConnection();
            if (!dbConn)
            {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody("Database connection not available");
                callback(resp);
                return;
            }

            // Execute query
            std::unique_ptr<sql::Statement> stmt(dbConn->createStatement());
            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT COUNT(*) as count FROM country"));

            std::string result;
            if (res->next())
            {
                result = res->getString("count");
                result += " countries";
            }
            else
            {
                result = "0 countries";
            }

            // Return response
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k200OK);
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody(result);
            callback(resp);
        }
        catch (sql::SQLException& e)
        {
            // Handle SQL exception
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);

            std::string errorMsg = "Database error: ";
            errorMsg += e.what();
            errorMsg += " (Code: " + std::to_string(e.getErrorCode()) + ")";

            resp->setBody(errorMsg);
            callback(resp);
        }
        catch (std::exception& e)
        {
            // Handle general exception
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);

            std::string errorMsg = "Error: ";
            errorMsg += e.what();

            resp->setBody(errorMsg);
            callback(resp);
        }
    }
};
