#define _WIN32_WINNT 0x0601
#include <crow.h>
#define STATIC_CONCPP
#include "config.hpp"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

sql::Driver* driver;
std::unique_ptr<sql::Connection> con;


std::string showPlayers()
{
  std::unique_ptr<sql::Statement> stmt;
  std::unique_ptr<sql::ResultSet> res;
  stmt.reset(con->createStatement());
  res.reset(stmt->executeQuery("SELECT COUNT(*) as count from country"));
  std::string result;
  while (res->next())
  {
    result += res->getString("count");
    result += " countries";
  }
  return  result;
}

int main()
{
  try
  {
    /* Create a connection */
    driver = get_driver_instance();
    //con.reset(driver->connect("testgorating.db", "kovarex", "zGZ*BqHeCW@!M"));
    con.reset(driver->connect(accessPoint.address, accessPoint.user, accessPoint.password));
    /* Connect to the MySQL test database */
     con->setSchema(accessPoint.database);
  }
  catch (sql::SQLException& e)
  {
    printf("# ERR: SQLException in %s", __FILE__);
    printf ("(%s) on line %u\n", __FUNCTION__, __LINE__);
    printf("# ERR: %s", e.what());
    printf(" (MySQL error code: %u", e.getErrorCode());
    printf(", SQLState: %s)\n", e.getSQLState().c_str());
    return 1;
  }

  crow::SimpleApp app;
  CROW_ROUTE(app, "/")(&showPlayers);
  app.port(18080).multithreaded().run();
  printf("Hello world\n");
}
