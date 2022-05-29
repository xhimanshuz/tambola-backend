#ifndef BOARDHANDLER_H
#define BOARDHANDLER_H

#include "Board/board.h"
#include "Handler/Handler.h"

namespace IO::DB::Object {
class Board;
}

class BoardHandler : public Handler {
  std::map<uint64_t, std::shared_ptr<Board>> boardMap;

 public:
  BoardHandler(HandlerManager* parent);

  std::shared_ptr<Board> getById(const uint64_t bid);
  std::shared_ptr<Board> getByUId(const uint64_t uid);
  std::optional<IO::DB::Object::Board> getByIdFromDB(uint64_t const bid);
  std::optional<IO::DB::Object::Board> getByUIdFromDB(uint64_t const uid);
  std::shared_ptr<Board> create(std::shared_ptr<User> user);
  std::shared_ptr<Board> create(
      std::shared_ptr<User> user,
      std::optional<IO::DB::Object::Board> const& board_db);
};

#endif  // BOARDHANDLER_H
