#ifndef USERSHANDLER_H
#define USERSHANDLER_H

#include "User/User.h"
#include "User/User_db.h"

#include <algorithm>
#include <optional>
#include <vector>
#include "Handler/Handler.h"

class UsersHandler : public Handler {
  std::map<uint64_t, std::shared_ptr<User>> _onlineUserListMap;

 public:
  UsersHandler(HandlerManager* parent);
  ~UsersHandler();

  void addToOnline(const std::shared_ptr<User>& user);
  void removeFromOnline(const std::shared_ptr<User>& user);
  std::shared_ptr<User> create(std::string name, std::string email);
  std::shared_ptr<User> getByEmailFromOnline(const std::string& email);
  std::optional<IO::DB::Object::User> getByEmailFromDB(
      const std::string& email);
  std::shared_ptr<User> getByIDFromOnline(const uint64_t id);
  std::optional<IO::DB::Object::User> getByIDFromDB(const uint64_t id);
  std::vector<std::shared_ptr<User>> getAllOnline() const;
  std::vector<IO::DB::Object::User> getAll();
  boost::json::object getAllJson();
};

#endif  // USERSHANDLER_H
