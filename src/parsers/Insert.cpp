#include "../../include/parsers/Insert.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Insert::parse () DEF_THROW {
    if (is("INSERT")) {
        push("insert");
        consume ();
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
    }

    else if (is("REPLACE")) {
        push("replace");
        consume ();

    }

    else {
        expect(oneOf("INSERT", "REPLACE"));
    }

    expect ("INTO");
    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        expect(DOT);
        add("table-name", expectName ());
    } else {
        add("table-name", expectName ());
    }



    pop();
}

} }


