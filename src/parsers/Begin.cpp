#include "../../include/parsers/Begin.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Begin::parse () DEF_THROW {
    push("begin");

    expect("BEGIN");

    if (hasNext () && is(oneOf("DEFERRED", "IMMEDIATE", "EXCLUSIVE"))) {
        add("modifier", next());
    }

    if (hasNext () && is("TRANSACTION")) {
        consume();
    }

    pop();
}

} }
