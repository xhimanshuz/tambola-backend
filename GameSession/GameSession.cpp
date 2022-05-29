#include "GameSession/GameSession.h"
#include "Board/board.h"
#include "GameSession/GameSession_db.h"
#include "RequestUtility.h"
#include "Ticket/Ticket.h"
#include "User/User.h"

// GameSession::GameSession(std::shared_ptr<Board> const& board)
//    : TObject(), _board{board} {}

GameSession::GameSession(std::optional<IO::DB::Object::GameSession> const& game_session_db,
                         const std::shared_ptr<Board>& board)
    : TObject(game_session_db->id), _board{board} {}

GameSession::GameSession(const uint64_t& id,
                         const std::shared_ptr<Board>& board)
    : TObject(id), _board{board} {}

GameSession::~GameSession() {}

void GameSession::add(std::shared_ptr<Ticket> ticket) {
  _ticketList.push_back(ticket);
}

boost::json::object GameSession::toJson() {
  boost::json::array ticket_ids;
  for (auto& ticket : _ticketList) {
    ticket_ids.push_back(ticket->getId().c_str());
  }
  return {{"id", _id},
          {"tickets_id", ticket_ids},
          {"board_id", _board->getId()},
          {"timestamp", getTimestamp()}};
}

void GameSession::numberPicked(uint16_t number) {
  auto update =
      RequestUtility().createUpdate("numberPicked", {{"number", number}});
  for (auto& ticket : _ticketList)
    ticket->sendToUser(update);
}

const std::shared_ptr<Board> &GameSession::board() const {
  return _board;
}

std::vector<std::string> GameSession::tids()
{
//  std::vector<uint16_t> tids_list;
//  for(auto ticket: _ticketList)
//    tids_list.push_back(ticket->getId());
//  return tids_list;
}
