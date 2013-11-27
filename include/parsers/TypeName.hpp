#ifndef TYPENAME_HPP
#define TYPENAME_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class TypeName : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // TYPENAME_HPP
