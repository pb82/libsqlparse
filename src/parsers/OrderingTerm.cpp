#include "../../include/parsers/OrderingTerm.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void OrderingTerm::parse () DEF_THROW {
    if (!isOrderingTerm ()) return;

    push("ordering-term");
    if (is("ORDER")) {
        parseOrderBy ();
    }

    if (is("LIMIT")) {
        parseLimit ();
    }

    pop();
}

void OrderingTerm::parseOrderBy () DEF_THROW {
    push ("order-by");

    expect ("ORDER");
    expect ("BY");

    parseTermList ();

    pop ();
}

void OrderingTerm::parseTermList () DEF_THROW {
    while (hasNext ()) {
        parseTerm ();
        if (hasNext () && is(COMMA)) {
            consume ();
            continue;
        } else {
            break;
        }
    }
}

void OrderingTerm::parseTerm () DEF_THROW {
    push ("ordering-term");

    getParser ("EXPRESSION").parse ();
    if (hasNext () && is ("COLLATE")) {
        consume ();
        add ("collation-name", expectName ());
    }

    if (hasNext () && is (oneOf("ASC", "DESC"))) {
        add ("ordering", next ());
    }

    pop ();
}

void OrderingTerm::parseLimit () DEF_THROW {
    expect ("LIMIT");
    getParser ("EXPRESSION").parse ();

    if (is(oneOf(",", "OFFSET"))) {
        consume ();
        push ("offset");
        getParser ("EXPRESSION").parse ();
    }
}

bool OrderingTerm::isOrderingTerm () const {
    return hasNext () && is (oneOf("ORDER", "LIMIT"));
}

} }

