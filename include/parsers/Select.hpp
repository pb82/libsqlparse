#ifndef SELECT_H
#define SELECT_H

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Select : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // SELECT_H
