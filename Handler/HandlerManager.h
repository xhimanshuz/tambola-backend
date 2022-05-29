#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "Board/BoardHandler.h"
#include "Database/PostgreSQL.h"
#include "GameSession/GameSessionHandler.h"
#include "Ticket/TicketHandler.h"
#include "User/UsersHandler.h"

class HandlerManager {
  IO::DB::Database* _db;

  HandlerManager();

 public:
  static HandlerManager* instance;
  static HandlerManager* get() {
    if (!instance)
      instance = new HandlerManager();
    return instance;
  }

  std::shared_ptr<BoardHandler> _board;
  std::shared_ptr<GameSessionHandler> _gameSession;
  std::shared_ptr<TicketHandler> _ticket;
  std::shared_ptr<UsersHandler> _user;

  const std::shared_ptr<BoardHandler>& board() const;
  const std::shared_ptr<TicketHandler>& ticket() const;
  const std::shared_ptr<UsersHandler>& user() const;
  const std::shared_ptr<GameSessionHandler>& gameSession() const;
  IO::DB::Database* getDB() const;
};

#endif  // HANDLERMANAGER_H
