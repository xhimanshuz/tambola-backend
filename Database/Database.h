#ifndef DATABASE_H
#define DATABASE_H

#include <fmt/core.h>

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class User;
class Board;
class Ticket;
class GameSession;

namespace IO::DB {
namespace Object {
  class Board;
  class User;
  class GameSession;
  class Ticket;
};  // namespace Object

class Database {
 public:
  std::string _connection_url;
  Database(const std::string& connection_url)
      : _connection_url{connection_url} {
    std::cout << "DataBase Connection URL: " << _connection_url;
  }

  template <typename T>
  std::string toArray(std::vector<T> vector) {
    if (vector.empty())
      return "{}";
    std::string data{"{"};
    for (auto& value : vector) {
      data += std::to_string(value) + ",";
    }
    data.pop_back();
    return data + "}";
  }

  std::string toArray(std::vector<std::string> vector) {
    if (vector.empty())
      return "{}";
    std::string data{"{"};
    for (auto& value : vector) {
      data += "\"" + value + "\",";
    }
    data.pop_back();
    return data + "}";
  }



  virtual void connect() = 0;
  virtual std::optional<IO::DB::Object::User> addUser(
      const std::shared_ptr<User>& user) = 0;
  virtual std::optional<IO::DB::Object::User> addUser(
      const std::string& email,
      const std::string& name) = 0;
  virtual std::optional<IO::DB::Object::User> addUser(const std::string& email,
                                                      const std::string& name,
                                                      const std::string& type,
                                                      uint64_t timestamp) = 0;
  virtual void modifyUser(const std::shared_ptr<User>& user) = 0;
  virtual std::optional<IO::DB::Object::User> getUserById(
      const uint64_t& id) = 0;
  virtual std::optional<IO::DB::Object::User> getUserByEmail(
      const std::string& email) = 0;
  virtual std::vector<IO::DB::Object::User> getAll() = 0;
  //  virtual uint64_t getNewUserId() = 0;

  virtual std::optional<IO::DB::Object::Board> addBoard(
      const std::shared_ptr<Board>& board) = 0;
  virtual std::optional<IO::DB::Object::Board> addBoard(
      const std::shared_ptr<User>& user) = 0;
  virtual std::optional<IO::DB::Object::Board> addBoard(
      const uint64_t uid,
      const std::vector<uint16_t>& number_on_board,
      const std::vector<uint16_t>& number_on_bucket,
      const uint64_t& gid,
      const uint64_t& timestamp) = 0;
  virtual void modifyBoard(std::shared_ptr<Board> board) = 0;
  virtual std::optional<IO::DB::Object::Board> getBoardById(
      const uint64_t& id) = 0;
  virtual std::optional<IO::DB::Object::Board> getBoardByUid(
      const uint64_t& uid) = 0;
  virtual std::optional<IO::DB::Object::Board> getBoardByTimestamp(
      const uint64_t& timestamp) = 0;
  virtual std::optional<IO::DB::Object::Board> updateBoard(
      std::shared_ptr<Board> const& board) = 0;

  virtual std::optional<IO::DB::Object::Ticket> addTicket(std::shared_ptr<Ticket> const& ticket) = 0;
  virtual std::optional<IO::DB::Object::Ticket> modifyTicket(std::shared_ptr<Ticket> const& ticket) = 0;
  virtual std::optional<IO::DB::Object::Ticket> getTicketById(std::string& id) = 0;
  virtual std::optional<IO::DB::Object::Ticket> getTicketByUid(uint64_t const& uid) = 0;
  virtual uint64_t getNewTicketSetId() = 0;

  virtual std::optional<IO::DB::Object::GameSession> addGameSession(
      std::shared_ptr<Board> const& board) = 0;
  virtual std::optional<IO::DB::Object::GameSession> modifyGameSession(
      std::shared_ptr<GameSession> const& gameSession) = 0;
  virtual std::optional<IO::DB::Object::GameSession> getGameSessionById(uint64_t const& id) = 0;
  virtual std::vector<std::optional<IO::DB::Object::GameSession>> getGameSessionByUid(uint64_t const& uid) = 0;
  virtual std::vector<std::optional<IO::DB::Object::GameSession>> getGameSessionByBid(uint64_t const& bid) = 0;
  virtual std::optional<IO::DB::Object::GameSession> getGameSessionByTimestamp(uint64_t const& timestamp) = 0;
};

}  // namespace IO::DB

#endif  // DATABASE_H
