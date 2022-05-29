#ifndef USER_DB_H
#define USER_DB_H

#include <stdint.h>
#include <string>

#include "Database/DBObject.h"

namespace IO::DB::Object {

/*
 *  id |          email          | password |       name       | type |
 *is_active |      timestamp
 *-----+-------------------------+----------+------------------+------+-----------+----------------------
 */
struct User : public IO::DB::Object::DBObject {
  uint64_t id;
  std::string email;
  std::string password;
  std::string name;
  std::string type;
  bool is_active;
  uint64_t timestamp;

  User(pqxx::row& row) {
    id = row["id"].as<uint64_t>();
    email = row["email"].as<std::string>();
    password = row["password"].as<std::string>();
    name = row["name"].as<std::string>();
    type = row["type"].as<std::string>();
    is_active = row["is_active"].as<bool>();
    timestamp = row["timestamp"].as<u_int64_t>();
  }

  boost::json::object toJson() override {
    return {{"id", id},     {"name", name},           {"email", email},
            {"type", type}, {"timestamp", timestamp}, {"is_active", is_active}};
  }
};
}  // namespace IO::DB::Object

#endif  // USER_DB_H
