#include "HandlerManager.h"
HandlerManager* HandlerManager::instance = nullptr;

HandlerManager::HandlerManager() {
  _db = new IO::DB::PostgreSQL("tambola_user", "", "localhost", "26257",
                               "tambola");
  _board = std::make_shared<BoardHandler>(this);
  _gameSession = std::make_shared<GameSessionHandler>(this);
  _ticket = std::make_shared<TicketHandler>(this);
  _user = std::make_shared<UsersHandler>(this);
}

const std::shared_ptr<GameSessionHandler>& HandlerManager::gameSession() const {
  return _gameSession;
}

IO::DB::Database* HandlerManager::getDB() const {
  return _db;
}

const std::shared_ptr<BoardHandler>& HandlerManager::board() const {
  return _board;
}

const std::shared_ptr<TicketHandler>& HandlerManager::ticket() const {
  return _ticket;
}

const std::shared_ptr<UsersHandler>& HandlerManager::user() const {
  return _user;
}
