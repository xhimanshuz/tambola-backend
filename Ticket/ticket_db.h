#ifndef TICKET_DB_H
#define TICKET_DB_H

#include "Database/DBObject.h"

namespace IO::DB::Object {

/*
 * // SQL SCHEMA
 *
 * CREATE TABLE tickets ( id TEXT primary key not null,
 *                      tsid INT not null default nextval('tsid'),
 *                      row_0 int[], row_1 INT[], row_2 INT[],
 *                      crow_0 int[], crow_1 INT[], crow_2 INT[],
 *                      uid INT REFERENCES users (id),
 *                      timestamp INT,
 *                      gid INT REFERENCES game_sessions (id));
 */

struct Ticket: public IO::DB::Object::DBObject {
  std::string id;
  uint64_t tsid;
  std::array<std::array<int, 5>, 3> numbers;
  std::array<std::array<int, 5>, 3> checked_numbers;
  uint64_t uid;
  uint64_t timestamp;
  uint64_t gid;

  Ticket(pqxx::row& row) {
    id = row["id"].as<std::string>();
    tsid = row["tsid"].as<uint64_t>();

    for(auto i = 0; i < numbers.size(); i++)
      toArray<5>(row["row_" + std::to_string(i)].as_array(), numbers[i]);
    for(auto i = 0; i < numbers.size(); i++)
      toArray<5>(row["crow_" + std::to_string(i)].as_array(), numbers[i]);

    uid = row["uid"].as<uint64_t>();
    timestamp = row["timestamp"].as<uint64_t>();
    gid = row["gid"].as<uint64_t>();
  }

  boost::json::object toJson() override {
    boost::json::array numbers_array;
    boost::json::array checked_numbers_array;
    for(auto jarr: {numbers_array, checked_numbers_array}) {
      for(auto row: numbers) {
        boost::json::array row_arr_json;
        for(auto n: row) {
          row_arr_json.push_back(n);
        }
      jarr.push_back(row_arr_json);
      }
    }

    return {
        {"id", id},
        {"tsid", tsid},
        {"numbers", numbers_array},
        {"checked_numbers", checked_numbers},
        {"uid", uid},
        {"timestamp", timestamp},
        {"gid", gid}
    };
  }

};

} // IO::DB::Object

#endif // TICKET_DB_H
