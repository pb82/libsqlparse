#include "../../include/parsers/Update.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Update::parse () DEF_THROW {
    push("update");

    expect("UPDATE");
    if (is("OR")) {
        consume ();
        add("alternative", expect(
                oneOf(
                    "ROLLBACK",
                    "ABORT",
                    "REPLACE",
                    "FAIL",
                    "IGNORE")));
    }


    getParser ("QUALIFIEDTABLE").parse ();

    expect("SET");
    parseUpdateColumns ();

    if (hasNext () && is("WHERE")) {
        consume ();
        push ("where");
        getParser ("EXPRESSION").parse ();
        pop ();
    }

    getParser ("ORDERINGTERM").parse ();

    pop();
}

void Update::parseUpdateColumns () DEF_THROW {
    while (hasNext ()) {
        push ("update-column");

        add("column-name", expectName ());
        expect("=");
        getParser ("EXPRESSION").parse ();

        pop ();
        if (hasNext () && is(COMMA)) {
            consume ();
            continue;
        } else {
            break;
        }
    }
}

} }

