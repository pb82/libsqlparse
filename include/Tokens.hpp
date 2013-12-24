#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <stack>

#include "./Token.hpp"
#include "./exceptions/EndOfStreamException.hpp"
#include "./exceptions/IllegalModificationException.hpp"

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
   * @brief has check if at least num items are left on the stream
   * @param num the minimum number of tokens that have to be left
   * on the stream for this method to return true
   * @return true if enough tokens left, false otherwise
   */
  inline bool has(unsigned int num) const {
      return index + num < tokens.size();
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
   * @brief peek return the token on the given index without
   * increment the stream
   * @return a reference to the selected token
   */
  inline const SqlToken& peek(unsigned int pos)
  const throw (Exceptions::EndOfStreamException) {
      if (index + pos >= tokens.size()) {
        throw Exceptions::EndOfStreamException();
      }

      return tokens.at(index + pos);
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
   * @brief getTokenCount return the number of tokens in the list
   * @return
   */
  unsigned int getTokenCount() const {
      return tokens.size ();
  }

  /**
   * @brief save saves the current position in the stream on a stack
   * the position can later be retrieved for backtracking purposes
   */
  void save() {
      streamPosition.push (index);
  }

  /**
   * @brief restore restores a previously saved position in the stream for
   * backtracking purposes
   */
  void restore() {
      this->index = streamPosition.top ();
      streamPosition.pop ();
  }

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
  std::stack<unsigned int> streamPosition;
};

}
#endif // TOKENS_HPP
