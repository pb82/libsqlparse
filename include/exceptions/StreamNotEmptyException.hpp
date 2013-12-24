#ifndef STREAMNOTEMPTYEXCEPTION_HPP
#define STREAMNOTEMPTYEXCEPTION_HPP

#include <stdexcept>

namespace Sql { namespace Exceptions {

class StreamNotEmptyException : public std::runtime_error {
public:
    StreamNotEmptyException()
    : std::runtime_error ("Tokens left at the end of parsing") {
    }
};

} }

#endif // STREAMNOTEMPTYEXCEPTION_HPP
