#include "../include/Tokens.hpp"

using namespace Sql;
using namespace Sql::Exceptions;

// Consistency check
void Tokens::checkStream() const throw (EndOfStreamException) {
  if (index >= tokens.size()) {
    throw EndOfStreamException();
  }
}

// Return reference to next token in stream
const SqlToken& Tokens::next() throw (EndOfStreamException) {
  checkStream();
  return tokens.at(index++);
}

// Still tokens in stream?
bool Tokens::hasNext() const {
  return index < tokens.size();
}

// Peek at the current item in the stream but do not increment the
// index
const SqlToken& Tokens::peek() const throw (EndOfStreamException) {
  checkStream();
  return tokens.at(index);
}

// Increment the index but do not return the token
void Tokens::consume() throw (EndOfStreamException) {
  checkStream();
  index++;
}

// Append tokens to the stream. Only permitted if no previous read
// operatio occurred.
void Tokens::feed(TOKEN code, const char *value)
  throw (IllegalModificationException) {
  if (0 < index) {
    throw IllegalModificationException();
  }
  tokens.push_back({code, value});
}

// Reset the stream and clear the tokens
void Tokens::reset() {
  tokens.clear();
  index = 0;
}

// Print the tokens to an external stream
namespace Sql {
    std::ostream& operator <<(std::ostream& stream, const Tokens& tokens) {
        for (unsigned int i = 0; i < tokens.tokens.size(); ++i) {
            const SqlToken& token = tokens.tokens.at(i);
            stream << "(" << token.code << ": " << token.value << ")" << std::endl;
        }
        return stream;
    }
}
