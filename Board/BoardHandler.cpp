#include "BoardHandler.h"
#include "Handler/HandlerManager.h"
#include "User/User.h"

BoardHandler::BoardHandler(HandlerManager* parent) : Handler(parent) {}

std::shared_ptr<Board> BoardHandler::getById(const uint64_t bid) {
    if (auto itr = boardMap.find(bid); itr != boardMap.end()) {
      return itr->second;
    }
  return nullptr;
}

std::shared_ptr<Board> BoardHandler::getByUId(const uint64_t uid) {
    if (auto itr = std::find_if(
            boardMap.begin(), boardMap.end(),
            [&uid](
                const std::pair<uint64_t, std::shared_ptr<Board>>&
                boardPair) {
              return boardPair.second->getUser()->getId() == uid;
            });
        itr != boardMap.end())
      return itr->second;

    return nullptr;
}

std::optional<IO::DB::Object::Board> BoardHandler::getByIdFromDB(uint64_t const bid)
{
  return parent()->getDB()->getBoardById(bid);
}

std::optional<IO::DB::Object::Board> BoardHandler::getByUIdFromDB(uint64_t const uid)
{
  return parent()->getDB()->getBoardByUid(uid);
}

std::shared_ptr<Board> BoardHandler::create(std::shared_ptr<User> user) {
  auto board = std::make_shared<Board>(user);
  boardMap[board->getId()] = board;
  return board;
}

std::shared_ptr<Board> BoardHandler::create(
    std::shared_ptr<User> user,
    const std::optional<IO::DB::Object::Board>& board_db) {
  return boardMap[board_db->id] = std::make_shared<Board>(user, board_db->id);
}
