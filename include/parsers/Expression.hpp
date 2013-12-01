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

    bool isLiteral() const;
    bool isCombinator() const;
};

} }

#endif // EXPRESSION_H
