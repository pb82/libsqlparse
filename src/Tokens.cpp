#include "../include/Tokens.hpp"

namespace Sql {
using namespace Exceptions;

// Reset the stream and clear the tokens
void Tokens::reset() {
  tokens.clear();
  index = 0;
}

// Append tokens to the stream. Only permitted if no previous read
// operatio occurred.
void Tokens::feed(TOKEN code, const char *value, unsigned int line)
  throw (IllegalModificationException) {
  if (0 < index) {
    throw IllegalModificationException();
  }
  tokens.push_back({code, value, line});
}

// Print the tokens to an external stream

std::ostream& operator <<(std::ostream& stream, const Tokens& tokens) {
    for (unsigned int i = 0; i < tokens.tokens.size(); ++i) {
        const SqlToken& token = tokens.tokens.at(i);
        stream << "(" << token.code << ": " << token.value << ")" << std::endl;
    }
    return stream;
}

}
