#ifndef UNKNONWSUBSETEXCEPTION_H
#define UNKNONWSUBSETEXCEPTION_H

namespace Sql { namespace Exceptions {

class UnknownSubsetException : public std::runtime_error {
private:
    unsigned int line;
public:
    UnknownSubsetException(const char *type, unsigned int line)
        : std::runtime_error(type), line(line) {
    }

    unsigned int getLine() {
      return line;
    }
};

} }

#endif // UNKNONWSUBSETEXCEPTION_H
