#include "../../include/parsers/Rollback.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Rollback::parse () DEF_THROW {
    push("rollback");

    expect("ROLLBACK");
    if (hasNext () && is("TRANSACTION")) {
        consume();
    }

    if (hasNext () && is("TO")) {
        consume ();
        if (is("SAVEPOINT")) {
            consume();
        }
        add("savepoint-name", expectName ());
    }

    pop();
}

} }

