#include "../../include/parsers/Attach.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Attach::parse () DEF_THROW {
    push("attach");

    expect("ATTACH");
    if (is("DATABASE")) {
        consume();
    }

    getParser ("EXPRESSION").parse ();
    expect("AS");

    add("database-name", expectName ());

    pop();
}

} }
