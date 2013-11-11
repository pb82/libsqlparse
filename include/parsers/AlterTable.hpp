#ifndef ALTERTABLE_HPP
#define ALTERTABLE_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class AlterTable : public BaseParser {
public:
    void parse(Node *const node) DEF_THROW;
};

} }

#endif // ALTERTABLE_HPP
