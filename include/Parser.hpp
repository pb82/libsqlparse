#ifndef PARSER_HPP
#define PARSER_HPP

#include <FlexLexer.h>
#include <sstream>

#include "./BaseParser.hpp"
#include "./exceptions/IllegalTokenException.h"

#define DEF_THROW throw (         \
  EndOfStreamException,           \
  IllegalModificationException,   \
  IllegalTokenException)

class Parser : public BaseParser {
public:
  void parse(const char* source) DEF_THROW;

  /**
   * @brief printTokensToStream get a string representation of the tokens
   * that are currently not consumed by the parser. Meant only for debugging
   * purposes
   * @param stream
   */
  void printTokensToStream(std::ostream& stream);
private:
  /**
   * @brief feedTokens create a token stream from the source input
   * throws IllegalModificationException when tokens are added but
   * the stream already had read access (consume, next)
   *
   * throws IllegalTokenException when the source string contains
   * a token that is not matched by one of the rules in the Lexer
   */
  void feedTokens(const char* source)
    throw (IllegalModificationException, IllegalTokenException);

  Node root;
};

#endif // PARSER_HPP
