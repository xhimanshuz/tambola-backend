#ifndef TICKET_H
#define TICKET_H

#include "TObject.h"
#include "User/UsersOwn.h"

class User;
class GameSession;

class Ticket : public TObject {
  std::string _id;
  uint64_t _setId;
  std::array<std::array<int, 9>, 3> _numbers;
  std::array<std::array<int, 9>, 3> _check_numbers;

  std::shared_ptr<User> _user;
  std::shared_ptr<GameSession> _gameSession;

 public:
  Ticket(uint64_t const setId, uint16_t const id);
  ~Ticket();

  bool pickNumber(uint16_t number);
  void setOwner(const std::shared_ptr<User>& user);
  std::shared_ptr<User> getUser();
  void sendToUser(const std::string& data) const;
  void sendToUser(const boost::json::object& json) const;
  void fillNumbers();

  uint64_t gid() const;

  boost::json::object toJson();

  uint64_t getSetId();
  const std::string &getId() const;
  void setGameSession(std::shared_ptr<GameSession> const& game_session);
  std::shared_ptr<GameSession> const& gameSession() const;
  const std::array<std::array<int, 9>, 3> &numbers() const;
  const std::array<std::array<int, 9>, 3> &check_numbers() const;
};

#endif  // TICKET_H

// id, set_id, numbers[3][5], checked_numbers, uid, gid;
