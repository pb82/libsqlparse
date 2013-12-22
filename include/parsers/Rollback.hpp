#ifndef ROLLBACK_HPP
#define ROLLBACK_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Rollback : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // ROLLBACK_HPP
