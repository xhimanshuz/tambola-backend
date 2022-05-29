#include "Ticket.h"
#include "WsSession.h"
#include <fmt/core.h>

Ticket::Ticket(uint64_t const setId, uint16_t const id) : TObject(), _setId{setId} {
  _id = fmt::format("{}_{}", _setId, id);
  fillNumbers();
}

const std::array<std::array<int, 9>, 3> &Ticket::numbers() const
{
  return _numbers;
}

const std::array<std::array<int, 9>, 3> &Ticket::check_numbers() const
{
  return _check_numbers;
}

Ticket::~Ticket()
{

}

bool Ticket::pickNumber(uint16_t number) {
  return true;
}

void Ticket::setOwner(const std::shared_ptr<User>& user) {
  _user = user;
}

std::shared_ptr<User> Ticket::getUser() {
  return _user;
}

void Ticket::sendToUser(const std::string& data) const {
  _user->getSession()->do_write(data);
}

void Ticket::sendToUser(const boost::json::object& json) const {
  _user->getSession()->do_write(json);
}

void Ticket::fillNumbers()
{
  _numbers[0] = {1,0,25,33,0,57,0,77,0};
  _numbers[1] = {0,17,0,37,0,58,0,78,81};
  _numbers[2] = {4,0,28,0,46,0,65,0,85};
}

uint64_t Ticket::gid() const {  return _gameSession->getId(); }

boost::json::object Ticket::toJson() {
  return {
      {"id", getId()},
      {"tsid", getSetId()},
      {"numbers", numbers()},
      {"checked_numbers", check_numbers()},
      {"uid", getUser()->getId()},
      {"timestamp", getTimestamp()},
      {"gid", getgid()}
  };
}

uint64_t Ticket::getSetId() {
  return _setId;
}

const std::string &Ticket::getId() const {
  return _id;
}

void Ticket::setGameSession(std::shared_ptr<GameSession> const& game_session) {
  _gameSession = game_session;
}

std::shared_ptr<GameSession> const& Ticket::gameSession() const {
  return _gameSession;
}
