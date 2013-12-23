#include "../../include/parsers/Create.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Create::parse () DEF_THROW {
    push("create");
    expect("CREATE");

    if (is(oneOf("UNIQUE", "INDEX"))) {
        parseCreateIndex ();
    }

    pop();
}

void Create::parseCreateIndex () DEF_THROW {
    if (is("UNIQUE")) {
        add("modifier", next ());
    }

    expect("INDEX");
    if (is("IF")) {
        expect("IF");
        expect("NOT");
        expect("EXISTS");
    }

    if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
        add("database-name", expectName ());
        consume ();
        add("index-name", expectName ());
    } else {
        add("index-name", expectName ());
    }

    expect("ON");
    add("table-name", expectName ());
    expect(LP);
    parseColumnList ();
    expect(RP);

    if (hasNext () && is("WHERE")) {
        consume ();
        push("where");
        getParser ("EXPRESSION").parse ();
        pop ();
    }
}

void Create::parseColumnList () DEF_THROW {
    while (hasNext ()) {
        parseIndexedColumn ();
        if (hasNext () && is(COMMA)) {
            consume();
            continue;
        } else {
            break;
        }
    }
}

void Create::parseIndexedColumn () DEF_THROW {
    push("indexed-column");
    add("name", expectName ());

    if (is("COLLATE")) {
        consume ();
        add("collation-name", expectName ());
    }

    if (is(oneOf("ASC", "DESC"))) {
        add("order", next ());
    }

    pop ();
}

} }

