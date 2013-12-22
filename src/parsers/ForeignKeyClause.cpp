#include "../../include/parsers/ForeignKeyClause.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void ForeignKeyClause::parse () DEF_THROW {
    push("references");

    expect("REFERENCES");
    add("foreign-table", expectName ());

    if (hasNext () && is(LP)) {
        consume ();
        parseColumnList ();
        expect(RP);
    }

    if (hasNext () && is (oneOf("ON","MATCH"))) {
        parseKeyBody ();
    }

    if (hasNext () && is (oneOf("NOT", "DEFERRABLE"))) {
        parseDeferrable ();
    }

    pop();
}

void ForeignKeyClause::parseDeferrable () DEF_THROW {
    push("deferrable");
    if (is ("NOT")) {
        add ("modifier", next());
    }

    expect ("DEFERRABLE");

    if (hasNext () && is("INITIALLY")) {
        consume ();
        add("initially", expect(oneOf("DEFERRED", "IMMEDIATE")));
    }

    pop ();
}

void ForeignKeyClause::parseKeyBody () DEF_THROW {
    if (!hasNext ()) {
        return;
    }

    if (is("MATCH")) {
        consume ();
        add("match", expectName ());
        parseKeyBody ();
    }

    else if (is("ON")) {
        consume ();
        push("on");
        add("action", expect(oneOf("DELETE", "UPDATE")));
        if (is("SET")) {
            add("do", next());
            add("parameter", expect(oneOf("NULL", "DEFAULT")));
        }

        else if (is("NO")) {
            add("do", next());
            add("parameter", expect("ACTION"));
        }

        else {
            add("do", expect(oneOf("CASCADE","RESTRICT")));
        }

        pop ();
        parseKeyBody ();
    }
}

void ForeignKeyClause::parseColumnList () DEF_THROW {
    while (hasNext ()) {
        add("column-name", expectName ());
        if (hasNext () && is(COMMA)) {
            consume();
            continue;
        } else {
            break;
        }
    }
}

} }


