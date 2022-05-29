#ifndef DBOBJECT_H
#define DBOBJECT_H

#include <boost/json.hpp>
#include <pqxx/pqxx>

namespace IO::DB::Object {

class DBObject {
 public:
  void toVector(pqxx::array_parser& array,
                std::vector<uint16_t>& vector) const {
    std::pair<pqxx::array_parser::juncture, std::string> data_map;
    for (auto current_juncture{pqxx::array_parser::juncture::row_start};
         current_juncture != pqxx::array_parser::juncture::done;
         data_map = array.get_next(), current_juncture = data_map.first) {
      if (current_juncture != pqxx::array_parser::juncture::string_value)
        continue;
      vector.push_back(std::atoi(data_map.second.c_str()));
    }
  }

  void toVector(pqxx::array_parser& array,
                std::vector<uint64_t>& vector) const {
    std::pair<pqxx::array_parser::juncture, std::string> data_map;
    for (auto current_juncture{pqxx::array_parser::juncture::row_start};
         current_juncture != pqxx::array_parser::juncture::done;
         data_map = array.get_next(), current_juncture = data_map.first) {
      if (current_juncture != pqxx::array_parser::juncture::string_value)
        continue;
      vector.push_back(std::atoi(data_map.second.c_str()));
    }
  }

  void toVector(pqxx::array_parser array,
                std::vector<std::string>& vector) const {
    std::pair<pqxx::array_parser::juncture, std::string> data_map;
    for (auto current_juncture{pqxx::array_parser::juncture::row_start};
         current_juncture != pqxx::array_parser::juncture::done;
         data_map = array.get_next(), current_juncture = data_map.first) {
      if (current_juncture != pqxx::array_parser::juncture::string_value)
        continue;
      vector.push_back(data_map.second);
    }
  }

  template<int N>
  void toArray(pqxx::array_parser array, std::array<int, N>& std_array) {
    std::pair<pqxx::array_parser::juncture, std::string> data_map;
    int i{0};
    for (auto current_juncture{pqxx::array_parser::juncture::row_start};
         current_juncture != pqxx::array_parser::juncture::done;
         data_map = array.get_next(), current_juncture = data_map.first) {
      if (current_juncture != pqxx::array_parser::juncture::string_value)
        continue;
      std_array[i++] = std::atoi(data_map.second.c_str());
    }
  }

  std::vector<uint16_t> toInt16Vector(pqxx::array_parser array) const {
    std::vector<uint16_t> vector;
    toVector(array, vector);
    return vector;
  }

  std::vector<uint64_t> toInt64Vector(pqxx::array_parser array) const {
    std::vector<uint64_t> vector;
    toVector(array, vector);
    return vector;
  }

//  template <typename T>
//  void toVector(pqxx::array_parser& array, std::vector<T>& vector) {
//    std::pair<pqxx::array_parser::juncture, std::string> data_map;
//    for (auto current_juncture{pqxx::array_parser::juncture::row_start};
//         current_juncture != pqxx::array_parser::juncture::done;
//         data_map = array.get_next(), current_juncture = data_map.first) {
//      T current_data;
//      if constexpr (typeid(T) == typeid(int)) {
//        current_data = std::atoi(data_map.second.c_str());
//      } else
//        current_data = data_map.second;
//      vector.push_back(current_data);
//    }
//  }

  virtual boost::json::object toJson() = 0;
};
}  // namespace IO::DB::Object
#endif  // DBOBJECT_H
