#ifndef FOREIGNKEYCLAUSE_HPP
#define FOREIGNKEYCLAUSE_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class ForeignKeyClause : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // FOREIGNKEYCLAUSE_HPP
