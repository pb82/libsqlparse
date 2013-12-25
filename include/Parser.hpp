#ifndef PARSER_HPP
#define PARSER_HPP

#include <FlexLexer.h>
#include <sstream>
#include <algorithm>

#include "./BaseParser.hpp"
#include "parsers/AlterTable.hpp"
#include "parsers/ColumnDef.hpp"
#include "parsers/TypeName.hpp"
#include "parsers/ColumnConstraint.hpp"
#include "parsers/ConflictClause.hpp"
#include "parsers/Expression.hpp"
#include "parsers/Select.hpp"
#include "parsers/ForeignKeyClause.hpp"
#include "parsers/Analyze.hpp"
#include "parsers/Attach.hpp"
#include "parsers/Begin.hpp"
#include "parsers/Commit.hpp"
#include "parsers/Rollback.hpp"
#include "parsers/Create.hpp"
#include "parsers/Update.hpp"
#include "parsers/Insert.hpp"
#include "parsers/Delete.hpp"
#include "parsers/QualifiedTableName.hpp"

namespace Sql {

class Parser : public BaseParser {
public:
  void parse() DEF_THROW;
  void reset();

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

  void feed(std::stringstream &source)
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
