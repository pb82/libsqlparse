#ifndef ALTERTABLE_HPP
#define ALTERTABLE_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class AlterTable : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // ALTERTABLE_HPP
