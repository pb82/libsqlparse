#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Expression : public BaseParser {
public:
    void parse() DEF_THROW;
private:
    void parseExpressionPrefix() DEF_THROW;
    void parseExpressionCore() DEF_THROW;
    void parseExpressionSuffix() DEF_THROW;
    void parseCombinator() DEF_THROW;
    void parseExpressionList() DEF_THROW;
    void parseWhenList() DEF_THROW;
    void parseBetween() DEF_THROW;
    void parseComparison() DEF_THROW;
    void parseInStatement() DEF_THROW;

    bool isLiteral() const;
    bool isCombinator() const;
    bool inBetween() const;

    /** super bad hack :(
      * consider:
      * '1 AND 2' is a (formally) valid expression
      * How should this be parsed:
      * 'BETWEEN 1 AND 2' ?
      * When parsing a between-statement we don't
      * treat 'AND' as a binary operator but as
      * part of the statement.
      * TODO: find a proper solution for this
      */

    std::stack<bool> between;
};

} }

#endif // EXPRESSION_H
