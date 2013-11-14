#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <vector>
#include <string>
#include <sstream>

#include "./Token.hpp"
#include "./exceptions/EndOfStreamException.h"
#include "./exceptions/IllegalModificationException.h"

namespace Sql {

struct SqlToken {
  TOKEN code;
  std::string value;
  unsigned int line;
};

class Tokens {
public:
  Tokens()
    : index(0) {
  }

  /**
   * @brief next return the current token and
   * increment the stream
   * @return a reference to the current token on the stream
   */
  inline const SqlToken& next() throw (Exceptions::EndOfStreamException) {
      checkStream();
      return tokens.at(index++);
  }

  /**
   * @brief hasNext returns true if there are tokens left in the stream and
   * a call to 'next' will not throw an exception. Returns false otherwise.
   * @return true if tokens left, false otherwise
   */
  inline bool hasNext() const {
      return index < tokens.size();
  }

  /**
   * @brief peek return the current token but do not
   * increment the stream
   * @return a reference to the current token in the stream
   */
  inline const SqlToken& peek() const throw (Exceptions::EndOfStreamException) {
      checkStream();
      return tokens.at(index);
  }

  /**
   * @brief consume increment the stream
   */
  inline void consume() throw (Exceptions::EndOfStreamException) {
      checkStream ();
      index++;
  }

  /**
   * @brief feed append tokens to the stream but ensure
   * that this happens before and tokens are pulled from
   * the stream
   * @param code
   * @param value
   */
  void feed(TOKEN code, const char* value, unsigned int line)
    throw (Exceptions::IllegalModificationException);

  /**
   * @brief reset clear the token collection and reset the
   * stream position variable
   */
  void reset();

  /**
   * @brief operator << allow printing a Tokens instance to a stream using the
   * '<<' operator (e.g. std::cout << tokens << std::endl). A call to this
   * function will not modify the tokens collection.
   * @param stream the stream to print to
   * @param tokens the tokens instance to be printed
   * @return a reference to the stream for further operations
   */
  friend std::ostream& Sql::operator<<(
          std::ostream& stream,
          const Tokens& tokens);

private:
  /**
   * @brief checkStream ensure there are tokens left
   * in the stream
   */
  inline void checkStream() const throw (Exceptions::EndOfStreamException) {
      if (index >= tokens.size()) {
        throw Exceptions::EndOfStreamException();
      }
  }

  unsigned int index;
  std::vector<SqlToken> tokens;
};

}
#endif // TOKENS_HPP
