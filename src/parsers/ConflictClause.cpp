#include "../../include/parsers/ConflictClause.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void ConflictClause::parse () DEF_THROW {
    if (!hasNext ())    return;
    if (!is("ON"))      return;

    expect("ON");
    expect("CONFLICT");

    push("conflict-clause");

    add("action",
            expect(
                oneOf(
                    "ROLLBACK",
                    "ABORT",
                    "FAIL",
                    "IGNORE",
                    "REPLACE")));

    pop();
}

} }

