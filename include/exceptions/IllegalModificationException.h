#ifndef ILLEGALMODIFICATIONEXCEPTION_H
#define ILLEGALMODIFICATIONEXCEPTION_H

#include <stdexcept>

class IllegalModificationException : public std::runtime_error {
public:
  IllegalModificationException()
    : std::runtime_error("Illegal modification of protected resource") {
  }
};

#endif // ILLEGALMODIFICATIONEXCEPTION_H
