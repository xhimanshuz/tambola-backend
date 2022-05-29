#ifndef USERS_H
#define USERS_H

#include "ORM/ORM_BASE.h"
#include <any>
#include <iostream>
#include <string>

namespace ORM {

class Users : public ORM_BASE {
  enum QUERY_TYPE { SELECT, INSERT, UPDATE };

public:
  Column<std::any> id{"id", ORM::ATTRIBUTE::PRIMARY_KEY};
  Column<const std::any> email{"email", ORM::ATTRIBUTE::PRIMARY_KEY};
  Column<const std::any> password{"password", ORM::ATTRIBUTE::NONE};
  Column<const std::any> name{"name", ORM::ATTRIBUTE::NONE};

protected:
  std::string columnsQueryString() override {}
  std::vector<std::any> columnsList() override {}

  bool executeQuery() override {
    try {
      if (!conn)
        throw "Connection not found";

      pqxx::work tx(*conn);

      pqxx::result result;
      switch (queryType) {
      case QUERY_TYPE::UPDATE:
        result = tx.exec_prepared("current_query", email, password, name);
        break;
      case QUERY_TYPE::INSERT:
        result = tx.exec_prepared("current_query", email, password, name);
        break;
      case QUERY_TYPE::SELECT:
        result = tx.exec_prepared("current_query");
      }

    } catch (std::exception const &e) {
      std::cerr << "[x] Exception: " << e.what();
      return false;
    }
    return true;
  }

  std::string getQuery() override { return query; }

  /**
   * @brief setQuery
   * @return
   */
  bool setQuery() override {
    try {
      switch (queryType) {
      case QUERY_TYPE::UPDATE:
        query = "UPDATE INTO users (email, password, name) VALUES ($1, $2, $3)"
                "WHERE id = $4;";
        break;
      case QUERY_TYPE::INSERT:
        query =
            "INSERT INTO users (email, password, name) VALUES (1$, $2, $3);";
        break;
      case QUERY_TYPE::SELECT:
        query = "SELECT id, email, password, name from users";
      }

      conn->prepare("current_query", query);
    } catch (std::exception const &e) {
      std::cout << __PRETTY_FUNCTION__ << e.what();
      return false;
    }
    return true;
  }

private:
  std::shared_ptr<pqxx::connection> conn;
  std::string query;
  QUERY_TYPE queryType;

  /**
   * @brief Users
   * @param _id
   * @param _email
   * @param _password
   * @param _name
   * @param _conn
   */
  Users(uint64_t _id, const std::string _email, const std::string _password,
        const std::string _name,
        std::shared_ptr<pqxx::connection> _conn = nullptr)
      : id{_id}, conn{_conn} {
    email = _email;
    password = _password;
    name = _name;
  }

  /**
   * @brief Users
   * @param _conn
   */
  Users(std::shared_ptr<pqxx::connection> _conn = nullptr) : conn{_conn} {}

  std::optional<Users> getUserByEmail(const std::string email) {
    pqxx::work tx(*conn);
    try {
      pqxx::row row = tx.exec1(
          "SELECT id, email, name FROM users WHERE email=" + tx.quote(email));
      if (row.empty())
        throw "User not found with email: " + email;

      auto user = std::make_optional<ORM::Users>(row["id"].as<uint64_t>(),
                                                 row["email"].as<std::string>(),
                                                 row["name"].as<std::string>());
      //      user->isUpdate = true;
      return user;
    } catch (std::exception const &e) {
      std::cerr << "[x] Exception occured: " << e.what() << std::endl;
      return std::nullopt;
    }
  }

  void setConnection(std::shared_ptr<pqxx::connection> _conn) { conn = _conn; }
};

} // namespace ORM

#endif // USERS_H
