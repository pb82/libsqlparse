#ifndef ATTACH_HPP
#define ATTACH_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Attach : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // ATTACH_HPP
