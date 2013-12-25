#ifndef INSERT_HPP
#define INSERT_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Insert : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // INSERT_HPP

