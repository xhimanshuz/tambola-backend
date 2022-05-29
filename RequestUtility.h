#ifndef REQUESTUTILITY_H
#define REQUESTUTILITY_H

#include <boost/json.hpp>
#include <chrono>
#include <string>

class RequestUtility {
 public:
  RequestUtility();

  boost::json::object createUpdate(const std::string& update_name,
                                   boost::json::object update);
};

#endif  // REQUESTUTILITY_H
