#ifndef TOBJECT_H
#define TOBJECT_H

#include <boost/json.hpp>

/**
 * @brief The Tambola Object class.
 *
 * This is a base class of tambola object.
 */
class TObject {
 public:
  const uint64_t _id;
  uint64_t _timestamp;

  explicit TObject() : _id{0} {
    _timestamp = std::chrono::system_clock::now().time_since_epoch().count();
  }
  TObject(uint64_t const& id) : _id{id} {
    _timestamp = std::chrono::system_clock::now().time_since_epoch().count();
  }
  TObject(uint64_t const& id, uint64_t const& timestamp)
      : _id{id}, _timestamp{timestamp} {}

  virtual ~TObject() {}

  virtual boost::json::object toJson() = 0;

  const uint64_t& getId() const { return _id; }
  uint64_t getTimestamp() const { return _timestamp; }
};

#endif  // TOBJECT_H
