#ifndef GAMESESSIONHANDLER_H
#define GAMESESSIONHANDLER_H

#include "GameSession/GameSession.h"
#include "GameSession/GameSession_db.h"
#include "Handler/Handler.h"

class Board;

class GameSessionHandler : public Handler {
  std::map<uint64_t, std::shared_ptr<GameSession>> _gameSessionMap;

 public:
  GameSessionHandler(HandlerManager* parent);
  std::shared_ptr<GameSession> getById(const uint64_t id);
  std::shared_ptr<GameSession> getByUid(const uint64_t uid);
  std::shared_ptr<GameSession> create(std::shared_ptr<Board> const& board);
  std::shared_ptr<GameSession> create(
      IO::DB::Object::GameSession gamesession_db,
      std::shared_ptr<Board> const& board);
};

#endif  // GAMESESSIONHANDLER_H
