#ifndef ILLEGALTOKENEXCEPTION_H
#define ILLEGALTOKENEXCEPTION_H

#include <stdexcept>
#include <string>

namespace Sql { namespace Exceptions {

class IllegalTokenException : public std::runtime_error {
public:
    IllegalTokenException(const char* token)
        : std::runtime_error(token) {
    }
};

} }
#endif // ILLEGALTOKENEXCEPTION_H
