#include "UsersOwn.h"

UsersOwn::UsersOwn(std::shared_ptr<User> const& user) : _user{user} {}

std::shared_ptr<User> const& UsersOwn::getUser() const {
  return _user;
}
