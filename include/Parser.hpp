#ifndef PARSER_HPP
#define PARSER_HPP

#include <FlexLexer.h>
#include <sstream>
#include <algorithm>

#include "./BaseParser.hpp"
#include "parsers/AlterTable.hpp"
#include "parsers/ColumnDef.hpp"
#include "parsers/TypeName.hpp"

namespace Sql {

class Parser : public BaseParser {
public:
  void parse() DEF_THROW;

  static std::map<std::string, std::unique_ptr<BaseParser>> parsers;

  /**
   * @brief feedTokens create a token stream from the source input
   * throws IllegalModificationException when tokens are added but
   * the stream already had read access (consume, next)
   *
   * throws IllegalTokenException when the source string contains
   * a token that is not matched by one of the rules in the Lexer
   */
  void feed(const char* source)
    throw (Exceptions::IllegalModificationException,
           Exceptions::IllegalTokenException);

  /**
   * @brief printTokensToStream get a string representation of the tokens
   * that are currently not consumed by the parser. Meant only for debugging
   * purposes
   * @param stream
   */
  void printTokensToStream(std::ostream& stream) const;
  void printSyntaxTree(std::ostream& stream) const;
private:
  void printSyntaxTreeInternal(std::ostream& stream, const Node *node) const;
  void registerSubsets() const;
  Node root;
};

}
#endif // PARSER_HPP
