#ifndef ILLEGALTOKENEXCEPTION_H
#define ILLEGALTOKENEXCEPTION_H

#include <stdexcept>
#include <string>

namespace Sql { namespace Exceptions {

class IllegalTokenException : public std::runtime_error {
private:
    unsigned int line;
public:  
    IllegalTokenException(const char* token, unsigned int line)
        : std::runtime_error(token), line(line) {
    }

    unsigned int getLine() {
      return line;
    }
};

} }
#endif // ILLEGALTOKENEXCEPTION_H
