#ifndef TEXCEPTION_H
#define TEXCEPTION_H

#include <stdint.h>
#include <string>

class TException {
  uint16_t _error_code;
  std::string _message;

 public:
  TException(uint16_t error_code, const std::string message)
      : _error_code{error_code}, _message{message} {}
  std::string message() const { return _message; }
  uint16_t error_code() const { return _error_code; }
};

#endif  // TEXCEPTION_H
