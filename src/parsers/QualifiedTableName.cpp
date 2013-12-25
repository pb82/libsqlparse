#include "../../include/parsers/QualifiedTableName.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void QualifiedTableName::parse () DEF_THROW {
    push("qualified-table-name");

    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        expect(DOT);
        add("table-name", expectName ());
    } else {
        add("table-name", expectName ());
    }

    if (hasNext () && is ("INDEXED")) {
        consume ();
        expect("BY");
        add("indexed-by", expectName ());
    }

    else if (hasNext () && is ("NOT")) {
        consume ();
        expect("INDEXED");
    }

    pop();
}

} }


