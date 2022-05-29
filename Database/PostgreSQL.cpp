#include "PostgreSQL.h"
#include <TException.h>
#include <exception>
#include "Board/board.h"
#include "GameSession/GameSession.h"
#include "GameSession/GameSession_db.h"
#include "Ticket/Ticket.h"
#include "Ticket/ticket_db.h"
#include "User/User.h"
#include "User/User_db.h"

IO::DB::PostgreSQL::PostgreSQL(const std::string& username,
                               const std::string& password,
                               const std::string& host,
                               const std::string& port,
                               const std::string& database_name)
    : Database("postgresql://" + username + ":" + password + "@" + host + ":" +
               port + "/" + database_name),
      _con{_connection_url} {}

std::optional<IO::DB::Object::User> IO::DB::PostgreSQL::addUser(
    const std::shared_ptr<User>& user) {
  auto email = user->getEmail();
  auto name = user->getName();
  auto timestamp = user->getTimestamp();
  auto type = user->getTypeStr();

  pqxx::work txn{_con};
  txn.exec_params0(
      "INSERT INTO tambola.Users (email, name, type, timestamp) VALUES ($1, "
      "$2, $3, $4);",
      email, name, type, timestamp);
  txn.commit();

  return getUserByEmail(email);
}

std::optional<IO::DB::Object::User> IO::DB::PostgreSQL::addUser(
    const std::string& email,
    const std::string& name) {
  return addUser(email, name, "NONE",
                 std::chrono::system_clock::now().time_since_epoch().count());
}

std::optional<IO::DB::Object::User> IO::DB::PostgreSQL::addUser(
    const std::string& email,
    const std::string& name,
    const std::string& type,
    uint64_t timestamp) {
  pqxx::work txn{_con};
  txn.exec_params0(
      "INSERT INTO tambola.Users (email, name, type, timestamp) VALUES ($1, "
      "$2, $3, $4);",
      email, name, type, timestamp);
  txn.commit();
  return getUserByEmail(email);
}

void IO::DB::PostgreSQL::connect() {}

void IO::DB::PostgreSQL::modifyUser(const std::shared_ptr<User>& user) {
  pqxx::work txn{_con};
  txn.exec_params0(
      "UPDATE tambola_db.Users SET email = $1, name = $2, timestamp = $3, type "
      "= $4, is_active = $5 WHERE "
      "id = $6",
      user->getEmail(), user->getName(), user->getTimestamp(),
      user->getTypeStr(), user->is_active(), user->getId());
  txn.commit();
}

std::optional<IO::DB::Object::User> IO::DB::PostgreSQL::getUserById(
    const uint64_t& id) {
  try {
    pqxx::work txn{_con};
    std::string query =
        "SELECT id, email, name, timestamp, type, is_active FROM tambola.User "
        "WHERE "
        "id = " +
        txn.quote(id);
    auto row{txn.exec1(query)};
    return std::make_optional<IO::DB::Object::User>(row);
  } catch (std::exception const& ex) {
    std::cerr << "[x] Exception occured: " << ex.what() << std::endl;
    return std::nullopt;
  }
}

std::optional<IO::DB::Object::User> IO::DB::PostgreSQL::getUserByEmail(
    const std::string& email) {
  pqxx::work txn{_con};
  try {
    std::string query =
        "SELECT id, email, password, name, type, timestamp, is_active FROM "
        "tambola.Users "
        "WHERE "
        "email = " +
        txn.quote(email);
    auto row{txn.exec1(query)};
    return std::make_optional<IO::DB::Object::User>(row);
  } catch (pqxx::unexpected_rows const& ex) {
    std::cerr << "[x] Exception unexpected row number occured: " << ex.what()
              << std::endl;
    return std::nullopt;
  } catch (std::exception const& ex) {
    throw TException(500, fmt::format("[x] Exception occured: {}", ex.what()));
  }
}

std::vector<IO::DB::Object::User> IO::DB::PostgreSQL::getAll() {
  pqxx::work txn{_con};
  std::vector<IO::DB::Object::User> user_vector;
  try {
    std::string query =
        "SELECT id, email, password, name, type, timestamp, is_active FROM "
        "tambola.Users;";

    auto result{txn.exec(query)};
    for (auto row : result) {
      user_vector.push_back(row);
    }

    return user_vector;
  } catch (std::exception const& ex) {
    std::cerr << "[x] Exceptions: " << ex.what() << std::endl;
    return {};
  }
}

std::optional<IO::DB::Object::Board> IO::DB::PostgreSQL::addBoard(
    const std::shared_ptr<Board>& board) {
  auto uid = board->getUser()->getId();
  auto number_on_board = board->getNumbersOnBoard();
  auto number_on_bucket = board->getNumbersOnBucket();
  auto gid = board->getGameSession()->getId();
  auto timestamp = board->getTimestamp();

  auto db_board =
      addBoard(uid, number_on_board, number_on_bucket, gid, timestamp);
  board->setId(db_board->id);
  return db_board;
}

std::optional<IO::DB::Object::Board> IO::DB::PostgreSQL::addBoard(
    const std::shared_ptr<User>& user) {
  pqxx::work txn{_con};
  auto row = txn.exec_params1("INSERT INTO boards ( uid, timestamp) VALUES ($1, $2) RETURNING *;",
                   user->getId(),
                   std::chrono::system_clock::now().time_since_epoch().count());
  txn.commit();
  return std::make_optional<IO::DB::Object::Board>(row);
}

void IO::DB::PostgreSQL::modifyBoard(std::shared_ptr<Board> board) {}

std::optional<IO::DB::Object::Board> IO::DB::PostgreSQL::getBoardById(
    const uint64_t& id) {
  pqxx::work txn{_con};
  auto row{txn.exec_params1(
      "SELECT id, uid, number_on_board, number_on_bucket, gid, timestamp "
      "FROM boards WHERE id = $1",
      id)};
  return IO::DB::Object::Board(row);
}

std::optional<IO::DB::Object::Board> IO::DB::PostgreSQL::getBoardByUid(
    const uint64_t& uid) {
  pqxx::work txn{_con};
  auto row{txn.exec_params1(
      "SELECT id, uid, number_on_board, number_on_bucket, gid, timestamp "
      "FROM boards WHERE uid = $1 ORDER BY timestamp LIMIT 1;",
      uid)};
  return IO::DB::Object::Board{row};
}

std::optional<IO::DB::Object::Board> IO::DB::PostgreSQL::getBoardByTimestamp(
    const uint64_t& timestamp) {
  pqxx::work txn{_con};
  auto row{txn.exec_params1(
      "SELECT id, uid, number_on_board, number_on_bucket, gid, timestamp "
      "FROM boards WHERE timestamp = $1",
      timestamp)};
  return IO::DB::Object::Board{row};
}

std::optional<IO::DB::Object::Board> IO::DB::PostgreSQL::updateBoard(
    const std::shared_ptr<Board>& board) {
  pqxx::work txn{_con};
  txn.exec_params0(
      "UPDATE boards set uid = $1, number_on_board = $2, number_on_bucket = "
      "$3, gid = $4, timestamp = $5 WHERE id = $6",
      board->getUser()->getId(), toArray(board->getNumbersOnBoard()),
      toArray(board->getNumbersOnBucket()), board->getGameSession()->getId(),
      board->getTimestamp(), board->getId());
  txn.commit();
  return getBoardById(board->getId());
}

std::optional<IO::DB::Object::Ticket> IO::DB::PostgreSQL::addTicket(const std::shared_ptr<Ticket> &ticket)
{
  auto id = ticket->getId();
  auto tsid = ticket->getSetId();
  auto row0 = ticket->numbers()[0];
  auto row1 = ticket->numbers()[1];
  auto row2 = ticket->numbers()[2];
  auto crow0 = ticket->check_numbers()[0];
  auto crow1 = ticket->check_numbers()[1];
  auto crow2 = ticket->check_numbers()[2];
  auto uid = ticket->getUser()->getId();
  auto timestamp = ticket->getTimestamp();
  auto gid = ticket->gid();

  pqxx::work txn{_con};
  auto row = txn.exec_params1("INSERT INTO tickets (id, tsid, row_0, row_1, row_2, crow_0, crow_1, crow_2, uid, timestamp, gid) "
      "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11) RETURNING *;", id, tsid, row0, row1, row2, crow0, crow1, crow2, uid, timestamp, gid);
  txn.commit();
  return std::make_optional<IO::DB::Object::Ticket>(row);
}

std::optional<IO::DB::Object::Ticket> IO::DB::PostgreSQL::modifyTicket(const std::shared_ptr<Ticket> &ticket)
{
  auto tsid = ticket->getSetId();
  auto row0 = ticket->numbers()[0];
  auto row1 = ticket->numbers()[1];
  auto row2 = ticket->numbers()[2];
  auto crow0 = ticket->check_numbers()[0];
  auto crow1 = ticket->check_numbers()[1];
  auto crow2 = ticket->check_numbers()[2];
  auto uid = ticket->getUser()->getId();
  auto timestamp = ticket->getTimestamp();
  auto gid = ticket->gid();

  pqxx::work txn{_con};
  auto row = txn.exec_params1("UPDATE tickets set tsid = $1, row_0 = $2, "
      "row_1 = $3, row_2 = $4, crow_0 = $5, crow_1 = $6, crow_2 = $7, uid = $8, timestamp = $9, gid = $10 RETURNING *",
      tsid, row0, row1, row2, crow0, crow1, crow2, uid, timestamp, gid);
  txn.commit();
  return std::make_optional<IO::DB::Object::Ticket>(row);
}

std::optional<IO::DB::Object::Ticket> IO::DB::PostgreSQL::getTicketById(std::string &id)
{
  pqxx::work txn{_con};
  auto row = txn.exec_params1("SELECT * FROM tickets WHERE id = $1", txn.quote(id));
  return std::make_optional<IO::DB::Object::Ticket>(row);
}

std::optional<IO::DB::Object::Ticket> IO::DB::PostgreSQL::getTicketByUid(const uint64_t &uid)
{
  pqxx::work txn{_con};
  auto row = txn.exec_params1("SELECT * FROM tickets WHERE uid = $1", uid);
  return std::make_optional<IO::DB::Object::Ticket>(row);
}

uint64_t IO::DB::PostgreSQL::getNewTicketSetId()
{
  pqxx::work txn{_con};
  auto row{txn.exec_params1("SELECT nextval('tsid');")};
  return row["nextval"].as<uint64_t>();
}

std::optional<IO::DB::Object::GameSession> IO::DB::PostgreSQL::addGameSession(
    std::shared_ptr<Board> const& board) {
  pqxx::work txn{_con};
  auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
  txn.exec_params0("INSERT INTO game_sessions (bid, timestamp) VALUES ($1, $2);",
                   board->getId(), timestamp);
  txn.commit();
  return getGameSessionByTimestamp(timestamp);
}

std::optional<IO::DB::Object::GameSession> IO::DB::PostgreSQL::modifyGameSession(
    const std::shared_ptr<GameSession>& gameSession) {
  pqxx::work txn{_con};
//  txn.exec_params0("UPDATE game_sessions SET bid = $1, tid = $2) VALUES ($1, $2, $3);",
//                   gameSession->board()->getId(), toArray(gameSession->tids()));
  txn.commit();
  return getGameSessionById(gameSession->getId());
}

std::optional<IO::DB::Object::GameSession>
IO::DB::PostgreSQL::getGameSessionById(const uint64_t& id) {
  pqxx::work txn{_con};
  auto row{txn.exec_params1("SELECT * FROM game_sessions WHERE id = $1", id)};
  return std::make_optional<IO::DB::Object::GameSession>(row);
}

std::vector<std::optional<IO::DB::Object::GameSession>>
IO::DB::PostgreSQL::getGameSessionByUid(uint64_t const& uid) {}

std::vector<std::optional<IO::DB::Object::GameSession>> IO::DB::PostgreSQL::getGameSessionByBid(uint64_t const& bid)
{
  pqxx::work txn{_con};
  auto result{txn.exec_params("SELECT * FROM game_sessions WHERE bid = $1", bid)};
  std::vector<std::optional<IO::DB::Object::GameSession>> game_session_list;
  for(auto const& row: result) {
    auto gs_db = std::make_optional<IO::DB::Object::GameSession>(row);
    game_session_list.push_back(gs_db);
  }
  return game_session_list;
}

std::optional<IO::DB::Object::GameSession> IO::DB::PostgreSQL::getGameSessionByTimestamp(uint64_t const& timestamp) {
  pqxx::work txn{_con};
  auto row{txn.exec_params1("SELECT * FROM game_sessions WHERE timestamp = $1", timestamp)};
  return std::make_optional<IO::DB::Object::GameSession>(row);
};

std::optional<IO::DB::Object::Board> IO::DB::PostgreSQL::addBoard(
    const uint64_t uid,
    const std::vector<uint16_t>& number_on_board,
    const std::vector<uint16_t>& number_on_bucket,
    const uint64_t& gid,
    const uint64_t& timestamp) {
  pqxx::work txn{_con};
  try {
    txn.exec_params0(
        "INSERT INTO boards ( uid, number_on_board, number_on_bucket, gid, "
        "timestamp ) VALUES "
        "( $1, $2, $3, $4, $5);",
        uid, toArray<uint16_t>(number_on_board),
        toArray<uint16_t>(number_on_bucket), gid, timestamp);
    txn.commit();
  } catch (std::exception const& ex) {
    throw TException(500, ex.what());
  }
  return getBoardByTimestamp(timestamp);
}
