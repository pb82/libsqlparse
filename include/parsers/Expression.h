#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Expression : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // EXPRESSION_H
