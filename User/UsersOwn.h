#ifndef USERSOWN_H
#define USERSOWN_H

#include "User/User.h"

class UsersOwn {
  std::shared_ptr<User> _user;

 public:
  UsersOwn(std::shared_ptr<User> const& user);
  std::shared_ptr<User> const& getUser() const;
};

#endif  // USERSOWN_H
