#include "User.h"
#include "Board/board.h"
#include "TException.h"
#include "Ticket/Ticket.h"
#include "User_db.h"
#include "WsSession.h"

User::User(const uint64_t id,
           std::string name,
           std::string email,
           TYPE type,
           uint64_t timestamp,
           bool is_active)
    : TObject{id},
      _name{name},
      _email{email},
      _type{type},
      _is_active{is_active},
      _session{nullptr},
      _gameSession{nullptr} {
  _timestamp = timestamp;
}

User::User(const uint64_t id,
           std::string name,
           std::string email,
           std::string type,
           uint64_t timestamp,
           bool is_active)
    : TObject{id, timestamp},
      _name{name},
      _email{email},
      _is_active{is_active},
      _session{nullptr},
      _gameSession{nullptr} {
  _type = (type == "")       ? TYPE::NONE
          : (type == "HOST") ? TYPE::HOST
                             : TYPE::PLAYER;
}

User::User(std::string name, std::string email, TYPE type)
    : TObject('u'),
      _name{name},
      _email{email},
      _type{type},
      _session{nullptr} {}

User::User(std::optional<IO::DB::Object::User>& user) : TObject(user->id) {
  if (!user)
    throw TException(500, "User not found in DB");
  setName(user->name);
  setEmail(user->email);
  setType(user->type);
  set_Is_active(user->is_active);
  setTimestamp(user->timestamp);
}

User::User(const User& user) : TObject(user.getId()) {
  _name = user.getName();
  _email = user.getEmail();
  _type = user.getType();
  _is_active = user.is_active();
  _session = user.getSession();
  _board = user.board();
  _ticket = user.ticket();
  _gameSession = user.getGameSession();
}

const std::shared_ptr<WsSession>& User::getSession() const {
  return _session;
}

void User::setSession(const std::shared_ptr<WsSession>& newSession) {
  _session = newSession;
}

const std::string User::getTid() const {
  return (_ticket) ? _ticket->getId() : "";
}

void User::set(std::shared_ptr<Ticket> ticket) {
  _ticket = ticket;
}

uint64_t User::getBid() const {
  return (_board) ? _board->getId() : 0;
}

void User::set(std::shared_ptr<Board> board) {
  _board = board;
}

boost::json::object User::toJson() {
  return {{"id", getId()},
          {"name", getName()},
          {"email", getEmail()},
          {"type", getTypeStr()},
          {"tid", getTid()},
          {"bid", getBid()},
          {"timestamp", getTimestamp()},
          {"is_active", is_active()}};
}

const std::shared_ptr<Board>& User::board() const {
  if (!_board)
    throw TException(404, "Board not found in current user.");
  return _board;
}

const std::shared_ptr<Ticket>& User::ticket() const {
  if (!_ticket)
    throw TException(404, "Ticket not found in current user.");
  return _ticket;
}

void User::setTicket(const std::shared_ptr<Ticket>& newTicket) {
  _ticket = newTicket;
}

void User::setBoard(const std::shared_ptr<Board>& newBoard) {
  _board = newBoard;
}

bool User::is_active() const {
  return _is_active;
}

void User::set_Is_active(bool newIs_active) {
  _is_active = newIs_active;
}

void User::setTimestamp(uint64_t newTimestamp) {
  _timestamp = newTimestamp;
}

User::TYPE User::getType() const {
  return _type;
}

void User::setType(TYPE newType) {
  _type = static_cast<TYPE>(_type | newType);
}

void User::setType(const std::string& type_str) {
  _type = (type_str == "PLAYER") ? TYPE::PLAYER
          : (type_str == "HOST") ? TYPE::HOST
                                 : TYPE::NONE;
}

std::string User::getTypeStr() {
  switch (_type) {
    case (TYPE::HOST | TYPE::PLAYER):
      return "HOST|PLAYER";
    case TYPE::PLAYER:
      return "PLAYER";
    case TYPE::HOST:
      return "HOST";
    default:
      return "NONE";
  }
}

const std::string& User::getEmail() const {
  return _email;
}

void User::setEmail(const std::string& newEmail) {
  _email = newEmail;
}

std::shared_ptr<GameSession> User::getGameSession() const {
  return _gameSession;
}

void User::set(std::shared_ptr<GameSession> gameSession) {
  _gameSession = gameSession;
}

const std::string& User::getName() const {
  return _name;
}

void User::setName(const std::string& newName) {
  _name = newName;
}
