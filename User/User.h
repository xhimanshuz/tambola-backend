#ifndef USER_H
#define USER_H

#include "TObject.h"

#include <stdint.h>
#include <chrono>
#include <memory>
#include <optional>
#include <string>

class WsSession;
class Board;
class Ticket;
class GameSession;

namespace IO::DB::Object {
class User;
}

class User : public TObject {
 public:
  enum TYPE { HOST = 0x01, PLAYER = 0x10, NONE = 0x00 };
  User(TYPE type, std::shared_ptr<WsSession>& session);
  User(const uint64_t id,
       std::string name,
       std::string email,
       User::TYPE type,
       uint64_t timestamp,
       bool is_active = true);

  User(const uint64_t id,
       std::string name,
       std::string email,
       std::string type,
       uint64_t timestamp,
       bool is_active = true);
  User(std::string name, std::string email, User::TYPE type = NONE);
  User(std::optional<IO::DB::Object::User>& user);
  User(User const& user);

  const std::string& getName() const;
  void setName(const std::string& newName);

  const std::string& getEmail() const;
  void setEmail(const std::string& newEmail);

  std::shared_ptr<GameSession> getGameSession() const;
  void set(std::shared_ptr<GameSession> gameSession);

  TYPE getType() const;
  void setType(TYPE newType);
  void setType(const std::string& type_str);
  std::string getTypeStr();

  void setTimestamp(uint64_t newTimestamp);

  const std::shared_ptr<WsSession>& getSession() const;
  void setSession(const std::shared_ptr<WsSession>& newSession);

  const std::string getTid() const;
  void set(std::shared_ptr<Ticket> ticket);

  uint64_t getBid() const;
  void set(std::shared_ptr<Board> board);

  boost::json::object toJson() override;

  const std::shared_ptr<Board>& board() const;

  const std::shared_ptr<Ticket>& ticket() const;

  void setTicket(const std::shared_ptr<Ticket>& newTicket);

  void setBoard(const std::shared_ptr<Board>& newBoard);

  bool is_active() const;
  void set_Is_active(bool newIs_active);

 private:
  std::string _name;
  std::string _email;
  TYPE _type;
  bool _is_active;

  std::shared_ptr<WsSession> _session;
  std::shared_ptr<Board> _board;
  std::shared_ptr<Ticket> _ticket;
  std::shared_ptr<GameSession> _gameSession;
};

#endif  // USER_H
