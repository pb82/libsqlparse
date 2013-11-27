#include "../../include/parsers/ColumnDef.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void ColumnDef::parse () DEF_THROW {
    push("coldef");
    add("column-name", expect(VALUE));

    if (is(VALUE)) {
        getParser ("TYPENAME").parse ();
    }

    pop();
}

} }
