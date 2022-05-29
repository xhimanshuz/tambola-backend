#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "TObject.h"
#include <optional>

class Ticket;
class Board;

namespace IO::DB::Object {
class GameSession;
}

class GameSession : public TObject {
 public:
  //  GameSession(std::shared_ptr<Board> const& board);
  GameSession(std::optional<IO::DB::Object::GameSession> const& game_session_db,
              std::shared_ptr<Board> const& board);
  GameSession(uint64_t const& id, std::shared_ptr<Board> const& board);
  ~GameSession();

  void add(std::shared_ptr<Ticket> ticket);
  boost::json::object toJson();
  void numberPicked(uint16_t number);

  const std::shared_ptr<Board> &board() const;
  std::vector<std::string> tids();

 private:
  std::vector<std::shared_ptr<Ticket>> _ticketList;
  std::shared_ptr<Board> _board;
};

#endif  // GAMESESSION_H
