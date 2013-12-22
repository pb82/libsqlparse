#include "../../include/parsers/Commit.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Commit::parse () DEF_THROW {
    push("commit");

    expect(oneOf("COMMIT", "END"));
    if (hasNext () && is ("TRANSACTION")) {
        consume ();
    }

    pop();
}

} }

