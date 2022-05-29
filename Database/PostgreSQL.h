#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include <Board/board_db.h>
#include <Database/Database.h>
#include <pqxx/pqxx>

namespace IO::DB {

class PostgreSQL : public IO::DB::Database {
  pqxx::connection _con;

 public:
  PostgreSQL(const std::string& username,
             const std::string& password,
             const std::string& host,
             const std::string& port,
             const std::string& database_name);

  std::optional<IO::DB::Object::User> addUser(
      const std::shared_ptr<User>& user) override;
  std::optional<IO::DB::Object::User> addUser(const std::string& email,
                                              const std::string& name) override;
  std::optional<IO::DB::Object::User> addUser(const std::string& email,
                                              const std::string& name,
                                              const std::string& type,
                                              uint64_t timestamp) override;
  void connect() override;
  void modifyUser(const std::shared_ptr<User>& user) override;
  std::optional<IO::DB::Object::User> getUserById(const uint64_t& id) override;
  std::optional<IO::DB::Object::User> getUserByEmail(
      const std::string& email) override;
  virtual std::vector<IO::DB::Object::User> getAll() override;

  std::optional<IO::DB::Object::Board> addBoard(
      const std::shared_ptr<Board>& board) override;
  std::optional<IO::DB::Object::Board> addBoard(
      const std::shared_ptr<User>& user) override;
  std::optional<IO::DB::Object::Board> addBoard(
      const uint64_t uid,
      const std::vector<uint16_t>& number_on_board,
      const std::vector<uint16_t>& number_on_bucket,
      const uint64_t& gid,
      const uint64_t& timestamp) override;
  void modifyBoard(std::shared_ptr<Board> board) override;
  std::optional<IO::DB::Object::Board> getBoardById(
      const uint64_t& id) override;
  std::optional<IO::DB::Object::Board> getBoardByUid(
      const uint64_t& uid) override;
  std::optional<IO::DB::Object::Board> getBoardByTimestamp(
      const uint64_t& timestamp) override;
  std::optional<IO::DB::Object::Board> updateBoard(
      std::shared_ptr<Board> const& board) override;

  std::optional<IO::DB::Object::Ticket> addTicket(std::shared_ptr<Ticket> const& ticket) override;
  std::optional<IO::DB::Object::Ticket> modifyTicket(std::shared_ptr<Ticket> const& ticket) override;
  std::optional<IO::DB::Object::Ticket> getTicketById(std::string& id) override;
  std::optional<IO::DB::Object::Ticket> getTicketByUid(uint64_t const& uid) override;
  uint64_t getNewTicketSetId() override;

  std::optional<IO::DB::Object::GameSession> addGameSession(
      std::shared_ptr<Board> const& board) override;
  std::optional<IO::DB::Object::GameSession> modifyGameSession(
      std::shared_ptr<GameSession> const& gameSession) override;
  std::optional<IO::DB::Object::GameSession> getGameSessionById(uint64_t const& id) override;
  std::vector<std::optional<IO::DB::Object::GameSession>> getGameSessionByUid(uint64_t const& uid) override;
  std::vector<std::optional<IO::DB::Object::GameSession>> getGameSessionByBid(uint64_t const& bid) override;
  std::optional<IO::DB::Object::GameSession> getGameSessionByTimestamp(uint64_t const& timestamp) override;
};
}  // namespace IO::DB
#endif  // POSTGRESQL_H
