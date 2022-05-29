#ifndef GAMESESSION_DB_H
#define GAMESESSION_DB_H

#include "Database/DBObject.h"

namespace IO::DB::Object {

class GameSession: IO::DB::Object::DBObject {
 public:
  uint64_t id;
  uint64_t timestamp;
  uint64_t bid;
  std::vector<std::string> tids;

  GameSession(pqxx::row const& row)
      : id{row["id"].as<uint64_t>()},
        timestamp{row["timestamp"].as<uint64_t>()},
        bid{row["bid"].as<uint64_t>()} {
    toVector(row["tids"].as_array(), tids);
  }

  boost::json::object toJson() {
    return {
        {"id", id},
        {"bid", bid},
        {"tids", tids},
        {"timestamp", timestamp}
    };
  }

};

} // IO::DB::Object

#endif // GAMESESSION_DB_H
