#include "GameSessionHandler.h"
#include "Board/board.h"
#include "Handler/HandlerManager.h"
#include "User/UsersHandler.h"

GameSessionHandler::GameSessionHandler(HandlerManager* parent)
    : Handler(parent) {}

std::shared_ptr<GameSession> GameSessionHandler::getById(const uint64_t id) {
  if (auto itr = _gameSessionMap.find(id); itr != _gameSessionMap.begin())
    return itr->second;
  return nullptr;
}

std::shared_ptr<GameSession> GameSessionHandler::getByUid(const uint64_t uid) {
  return parent()->user()->getByIDFromOnline(uid)->getGameSession();
}

std::shared_ptr<GameSession> GameSessionHandler::create(
    std::shared_ptr<Board> const& board) {
  auto game_session_db = parent()->getDB()->addGameSession(board);
  auto gameSession = std::make_shared<GameSession>(game_session_db, board);
  _gameSessionMap[gameSession->getId()] = gameSession;
  board->setGameSession(gameSession);
  return gameSession;
}

std::shared_ptr<GameSession> GameSessionHandler::create(
    IO::DB::Object::GameSession gamesession_db,
    const std::shared_ptr<Board>& board) {
  return std::make_shared<GameSession>(gamesession_db, board);
}
