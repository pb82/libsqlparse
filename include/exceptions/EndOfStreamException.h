#ifndef ENDOFSTREAMEXCEPTION_H
#define ENDOFSTREAMEXCEPTION_H

#include <stdexcept>

class EndOfStreamException : public std::runtime_error {
public:
  EndOfStreamException()
    : std::runtime_error ("Unexpected end of stream") {
  }
};

#endif // ENDOFSTREAMEXCEPTION_H
