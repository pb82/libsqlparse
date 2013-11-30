#include "../../include/parsers/ColumnDef.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void ColumnDef::parse () DEF_THROW {
    push("coldef");
    add("column-name", expect(VALUE));

    if (hasNext () && is(VALUE)) {
        getParser ("TYPENAME").parse ();
    }

    while (hasNext () && isConstraint ()) {
        getParser ("COLCONSTR").parse ();
    }

    pop();
}

bool ColumnDef::isConstraint () const {
    if (!hasNext ()) {
        return false;
    }

    return is(oneOf(
                  "CONSTRAINT",
                  "PRIMARY",
                  "NOT",
                  "UNIQUE",
                  "CHECK",
                  "DEFAULT",
                  "COLLATE",
                  "REFERENCES"));
}

} }
