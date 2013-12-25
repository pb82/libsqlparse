#include "../../include/parsers/Delete.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Delete::parse () DEF_THROW {
    push("delete");

    expect("DELETE");
    expect("FROM");

    getParser ("QUALIFIEDTABLE").parse ();

    if (hasNext () && is ("WHERE")) {
        consume ();
        push("where");
        getParser ("EXPRESSION").parse ();
        pop ();
    }

    pop();
}

} }



