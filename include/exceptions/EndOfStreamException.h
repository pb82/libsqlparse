#ifndef ENDOFSTREAMEXCEPTION_H
#define ENDOFSTREAMEXCEPTION_H

#include <stdexcept>

namespace Sql { namespace Exceptions {

class EndOfStreamException : public std::runtime_error {
public:
    EndOfStreamException()
    : std::runtime_error ("Unexpected end of stream") {
    }
};

} }
#endif // ENDOFSTREAMEXCEPTION_H
