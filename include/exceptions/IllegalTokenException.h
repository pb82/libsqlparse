#ifndef ILLEGALTOKENEXCEPTION_H
#define ILLEGALTOKENEXCEPTION_H

#include <stdexcept>
#include <string>

class IllegalTokenException : public std::runtime_error {
public:
  IllegalTokenException(const char* token)
    : std::runtime_error(token) {
  }
};

#endif // ILLEGALTOKENEXCEPTION_H
