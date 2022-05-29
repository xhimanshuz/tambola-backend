#ifndef ORM_BASE_H
#define ORM_BASE_H

#include <pqxx/pqxx>
#include <string>

#include "Column.h"

namespace ORM {

class ORM_BASE {
public:
  ORM_BASE() {}

protected:
  virtual bool createTable() = 0;
  virtual bool executeQuery() = 0;
  virtual std::string getQuery() = 0;
  virtual bool setQuery();
  virtual std::string columnsQueryString() = 0;
  virtual std::vector<std::any> columnsList() = 0;
};

} // namespace ORM
#endif // ORM_BASE_H
