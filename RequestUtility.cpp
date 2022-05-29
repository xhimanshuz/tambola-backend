#include "RequestUtility.h"

RequestUtility::RequestUtility() {}

boost::json::object RequestUtility::createUpdate(const std::string& update_name,
                                                 boost::json::object update) {
  return {{"type", "update"},
          {"timestamp",
           std::chrono::system_clock::now().time_since_epoch().count()},
          {"name", update_name},
          {"update", update}};
}
