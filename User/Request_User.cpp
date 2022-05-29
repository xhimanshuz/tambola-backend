#include "WsSession.h"

const std::shared_ptr<User>& WsSession::currentUser() const {
  if (!_currentUser)
    throw TException(503, "Unauthorized access, Sign In first.");
  return _currentUser;
}

json::object WsSession::signUp(json::object& request) {
  if (_currentUser)
    throw TException(500, "Already Signed In, Please Logout first");
  auto email = request["email"].as_string().c_str();
  auto password = request["password"].as_string().c_str();
  auto name = request["name"].as_string().c_str();

  auto user = handler->user()->create(name, email);
  return createResponse(request, user->toJson());
}

json::object WsSession::signIn(json::object& request) {
  if (_currentUser)
    throw TException(500, "Already Signed In, Please Logout first");
  if (request["signin_type"].as_string() == "manual") {
    return manualSignIn(request);
  }

  throw TException(400, "Sign In type not found.");
}

json::object WsSession::manualSignIn(json::object& request) {
  auto email = request["email"].as_string().c_str();
  auto user_db = handler->user()->getByEmailFromDB(email);

  auto user = std::make_shared<User>(user_db);
  handler->user()->addToOnline(user);
  if (!user)
    throw TException(404, "User not found with email");

  user->setSession(shared_from_this());
  setCurrentUser(user);
  return createResponse(request, user->toJson());
}

json::object WsSession::logout(json::object& request) {
  handler->user()->removeFromOnline(currentUser());
  _currentUser.reset();
  return createResponse(request, {});
}

json::object WsSession::getAllUsers(json::object& request) {
  return createResponse(request, handler->user()->getAllJson());
}
