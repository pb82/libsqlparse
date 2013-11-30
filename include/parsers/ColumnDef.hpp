#ifndef COLUMNDEF_HPP
#define COLUMNDEF_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class ColumnDef : public BaseParser {
public:
    void parse() DEF_THROW;

private:
    bool isConstraint() const;
};

} }

#endif // COLUMNDEF_HPP
