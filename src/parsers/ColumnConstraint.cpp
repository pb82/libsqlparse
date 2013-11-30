#include "../../include/parsers/ColumnConstraint.h"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void ColumnConstraint::parse () DEF_THROW {
    push("col-constraint");

    if(is("CONSTRAINT")) {
        consume ();
        add("constraint-name", expect(VALUE));
    }

    if (is("PRIMARY")) {
        add("constraint-type", expect(KEYWORD));
        expect("KEY");

        if (hasNext () && is(oneOf("ASC", "DESC"))) {
            add("ordering", expect(KEYWORD));
        }

        getParser ("CONFLICT").parse ();

        if (hasNext () && is ("AUTOINCREMENT")) {
            add ("autoincrement", expect(KEYWORD));
        }
    }

    else if (is("NOT")) {
        add("constraint-type", expect(KEYWORD));
        expect("NULL");
        getParser ("CONFLICT").parse ();
    }

    else if (is("UNIQUE")) {
        add("constraint-type", expect(KEYWORD));
        getParser ("CONFLICT").parse ();
    }

    else if (is("CHECK")) {
        add("constraint-type", expect(KEYWORD));
        expect("(");
        getParser ("EXPRESSION").parse ();
        expect(")");
    }

    pop();
}

} }

