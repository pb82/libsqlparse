#ifndef CONFLICTCLAUSE_H
#define CONFLICTCLAUSE_H

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class ConflictClause : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // CONFLICTCLAUSE_H
