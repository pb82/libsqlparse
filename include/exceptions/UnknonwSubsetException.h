#ifndef UNKNONWSUBSETEXCEPTION_H
#define UNKNONWSUBSETEXCEPTION_H

namespace Sql { namespace Exceptions {

class UnknownSubsetException : public std::runtime_error {
public:
    UnknownSubsetException(const char *type)
        : std::runtime_error(type) {
    }
};

} }

#endif // UNKNONWSUBSETEXCEPTION_H
