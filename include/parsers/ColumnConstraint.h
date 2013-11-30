#ifndef COLUMNCONSTRAINT_H
#define COLUMNCONSTRAINT_H

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class ColumnConstraint : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }
#endif // COLUMNCONSTRAINT_H
