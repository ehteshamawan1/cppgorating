#include <string>

class AccessPoint
{
public:
  AccessPoint(std::string_view address,
              std::string_view user,
              std::string_view database,
              std::string_view password)
    : address(address), user(user), database(database), password(password) {}

  std::string address;
  std::string user;
  std::string database;
  std::string password;
};
