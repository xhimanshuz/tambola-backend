#ifndef BOARD_DB_H
#define BOARD_DB_H

#include "Database/DBObject.h"

namespace IO::DB::Object {

struct Board : public DBObject {
  uint64_t id;
  uint64_t uid;
  uint64_t gid;
  std::vector<uint16_t> number_on_board;
  std::vector<uint16_t> number_on_bucket;
  uint64_t timestamp;

  //  Board(const pqxx::row& row) {
  //    id = row["id"].as<uint64_t>();
  //    uid = row["uid"].as<uint64_t>();
  //    gid = row["gid"].as<uint64_t>();
  //    number_on_board = toIntVector(row["number_on_board"].as_array());
  //    number_on_bucket = toIntVector(row["number_on_bucket"].as_array());
  //    timestamp = row["timestamp"].as<uint64_t>();
  //  }

  Board(const pqxx::row& row)
      : id{row["id"].as<uint64_t>()},
        uid{row["uid"].as<uint64_t>()},
        gid{row["gid"].as<uint64_t>()},
        number_on_board{toInt16Vector(row["number_on_board"].as_array())},
        number_on_bucket{toInt16Vector(row["number_on_bucket"].as_array())},
        timestamp{row["timestamp"].as<uint64_t>()} {}

  boost::json::object toJson() override {
    auto sorted_number_on_board = number_on_board;
    std::sort(sorted_number_on_board.begin(), sorted_number_on_board.end());

    return {{"board_id", id},
            {"numbers_on_board", sorted_number_on_board},
            {"last_picked_number", number_on_board.back()},
            {"remaining_numbers", number_on_bucket},
            {"picked_numbers_in_sequence", number_on_board}};
  }
};
}  // namespace IO::DB::Object

#endif  // BOARD_DB_H
