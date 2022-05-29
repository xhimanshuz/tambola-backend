#ifndef COLUMN_H
#define COLUMN_H

#include <any>
#include <string>

namespace ORM {

enum ATTRIBUTE { NONE, PRIMARY_KEY, SERIAL, AUTO_INCREMENT };

template <typename T> class Column : public std::any {

public:
private:
  std::string m_name;
  ATTRIBUTE m_attrib;

public:
  Column() = delete;
  explicit Column(const std::string _name, ATTRIBUTE _attrib = ATTRIBUTE::NONE)
      : m_name{_name} {
    m_attrib = _attrib;
  }

  virtual ~Column(){};
  std::string name() { return m_name; }

  constexpr Column(T const &&t) noexcept : std::any{t} {};
  constexpr bool valid() const noexcept { return has_value(); }
  constexpr bool invalid() const noexcept { return !has_value(); }
  constexpr T const get() const noexcept {
    return valid() ? std::any_cast<T>(*this) : T();
  }

  /**
   * @brief operator =
   * @param value
   */
  void operator=(T const &value) { std::any{value} = value; }
};
} // namespace ORM

#endif // COLUMN_H
