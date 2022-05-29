#include "UsersHandler.h"
#include "Handler/HandlerManager.h"
#include "TException.h"

UsersHandler::UsersHandler(HandlerManager* parent) : Handler(parent) {}

UsersHandler::~UsersHandler() {}

void UsersHandler::addToOnline(const std::shared_ptr<User>& user) {
  if (getByEmailFromOnline(user->getEmail()))
    throw TException(500, "User Already SignedIn");

  _onlineUserListMap[user->getId()] = user;
}

void UsersHandler::removeFromOnline(const std::shared_ptr<User>& user) {
  if (!getByIDFromOnline(user->getId()))
    throw TException(404, fmt::format("User with email: {} is not Online.",
                                      user->getEmail()));
  _onlineUserListMap.erase(user->getId());
}

std::shared_ptr<User> UsersHandler::create(std::string name,
                                           std::string email) {
  auto user_db = parent()->getDB()->addUser(email, name);
  auto user = std::make_shared<User>(user_db);
  return user;
}

// std::shared_ptr<TObject> UsersHandler::add(TObject* tObject) {
//  auto user = static_cast<User*>(tObject);
//  if (getByEmail(user->getEmail()) || getByID(user->getId()))
//    throw "User already exist";

//  userList.push_back(*user);
//  return userList.back();
//}

std::shared_ptr<User> UsersHandler::getByEmailFromOnline(
    const std::string& email) {
  if (auto user_it = find_if(
          _onlineUserListMap.begin(), _onlineUserListMap.end(),
          [&email](
              std::pair<uint64_t, std::shared_ptr<User>> const& user_pair) {
            return user_pair.second->getEmail() == email;
          });
      user_it != _onlineUserListMap.end()) {
    return user_it->second;
  }
  return nullptr;
}

std::optional<IO::DB::Object::User> UsersHandler::getByEmailFromDB(
    const std::string& email) {
  return parent()->getDB()->getUserByEmail(email);
}

std::shared_ptr<User> UsersHandler::getByIDFromOnline(const uint64_t id) {
  auto user_db = getByIDFromDB(id);
  if (auto user_pair_it = _onlineUserListMap.find(id);
      user_pair_it != _onlineUserListMap.end()) {
    return user_pair_it->second;
  }
  return nullptr;
}

std::optional<IO::DB::Object::User> UsersHandler::getByIDFromDB(
    const uint64_t id) {
  return parent()->getDB()->getUserById(id);
}

std::vector<std::shared_ptr<User>> UsersHandler::getAllOnline() const {
  std::vector<std::shared_ptr<User>> user_online_list;
  for (auto const& user_pair : _onlineUserListMap)
    user_online_list.push_back(user_pair.second);
  return user_online_list;
}

std::vector<IO::DB::Object::User> UsersHandler::getAll() {
  return parent()->getDB()->getAll();
}

boost::json::object UsersHandler::getAllJson() {
  boost::json::array online_user_array;
  for (auto user : getAllOnline()) {
    online_user_array.push_back(user->toJson());
  }

  boost::json::array all_users_array;
  for (auto user : getAll()) {
    all_users_array.push_back(user.toJson());
  }

  boost::json::object json{{"online", {{"user", online_user_array}}},
                           {"all", {{"user", all_users_array}}}};
  return json;
}
