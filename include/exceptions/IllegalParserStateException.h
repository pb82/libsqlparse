#ifndef ILLEGALPARSERSTATEEXCEPTION_H
#define ILLEGALPARSERSTATEEXCEPTION_H
#include <stdexcept>

namespace Sql { namespace Exceptions {

class IllegalParserStateException : public std::runtime_error {
public:
    IllegalParserStateException()
    : std::runtime_error("Illegal state at the end of parsing") {
    }
};

} }
#endif // ILLEGALPARSERSTATEEXCEPTION_H
