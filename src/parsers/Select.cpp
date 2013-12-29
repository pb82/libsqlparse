#include "../../include/parsers/Select.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Select::parse () DEF_THROW {
    while (hasNext ()) {
        parseSelectCore ();
        if (hasNext () && isCompoundOperator ()) {
            continue;
        } else {
            break;
        }
    }

    getParser ("ORDERINGTERM").parse ();
}

bool Select::isCompoundOperator () const {
    return hasNext () && is(oneOf("UNION", "INTERSECT", "EXCEPT"));
}

void Select::parseCompoundOperator () DEF_THROW {
    push("compound-operator");
    if (is("UNION")) {
        add ("type", next ());
        if (hasNext () && is ("ALL")) {
            add("modifier", next ());
        }
    }

    expect(oneOf("INTERSECT", "EXCEPT"));
}

void Select::parseSelectCore () DEF_THROW {
    push("select-core");
    expect("SELECT");
    if (is(oneOf("DISTINCT", "ALL"))) {
        add("modifier", expect(oneOf("DISTINCT", "ALL")));
    }

    parseResultColList ();

    if (hasNext () && is("FROM")) {
        push ("from");
        parseJoinSource ();
        pop ();
    }

    if (hasNext () && is("WHERE")) {
        consume ();
        push ("where");
        getParser ("EXPRESSION").parse ();
        pop ();
    }

    if (hasNext () && is("GROUP")) {
        push("group-by");
        parseGroupBy ();
        pop ();
    }

    pop();

}

void Select::parseGroupBy () DEF_THROW {
    expect("GROUP");
    expect("BY");
    parseExpressionList ("group-by-expression");

    if (hasNext () && is("HAVING")) {
        consume ();
        getParser ("EXPRESSION").parse ();
    }
}

void Select::parseJoinSource () DEF_THROW {
    expect ("FROM");
    parseSingleSource ();
    if (isJoinOp ()) {
        parseJoinOpList ();
    }
}

void Select::parseSingleSource () DEF_THROW {
    push ("single-source");

    if (is(LP)) {
        consume ();
        if (is("SELECT")) {
            getParser ("SELECT").parse ();
            expect (RP);
            if (is ("AS")) {
                consume ();
                add("table-alias", expectName ());
            }
        } else {
            parseJoinSource ();
        }
    }

    else {
        if (has(2) && isName (0) && is(1, DOT) && isName (2)) {
            add("database-name", expectName ());
            expect(DOT);
            add("table-name", expectName ());
        } else {
            add("table-name", expectName ());
        }

        if (hasNext () && is("AS")) {
            push ("as");
            consume ();
            add ("alias", next());
            pop ();
        }

        if (hasNext () && is("INDEXED")) {
            consume ();
            expect ("BY");
            add("index-name", expectName ());
        }

        else if (hasNext () && is("NOT")) {
            expect ("INDEXED");
        }
    }

    pop ();
}

void Select::parseJoinOpList () DEF_THROW {
    while (isJoinOp ()) {
        parseJoinOp ();
        parseSingleSource ();
        if (hasNext () && is (oneOf("ON", "USING"))) {
            parseJoinConstraint ();
        }
    }
}

void Select::parseJoinConstraint () DEF_THROW {
    push ("join-constraint");

    if (is("ON")) {
        push ("on");
        consume ();
        getParser ("EXPRESSION").parse ();
        pop ();
    } else {
        push ("using");
        expect ("USING");
        expect(LP);
        parseNameList ("column-name");
        expect (RP);
        pop ();
    }

    pop ();
}

void Select::parseJoinOp () DEF_THROW {
    push ("join-op");

    if (is(COMMA)) {
        consume ();
        pop ();
        return;
    }

    if (is("NATURAL")) {
        add ("natural", next ());
    }

    if (is("JOIN")) {
        add("type", next());
    }

    else if (is("LEFT")) {
        add("type", next());
        if (is("OUTER")) {
            add ("outer", next ());
        }

        expect ("JOIN");
    }

    else if (is("INNER")) {
        add("type", next());
        expect("JOIN");
    }

    else if (is("CROSS")) {
        add("type", next());
        expect("JOIN");
    }

    pop ();
}

bool Select::isJoinOp () const {
    if (!hasNext ()) {
        return false;
    }

    if (is(COMMA)) {
        return true;
    }

    if (is(oneOf("NATURAL", "JOIN", "LEFT", "INNER", "CROSS"))) {
        return true;
    }

    return false;
}

void Select::parseResultColList () DEF_THROW {
    while (hasNext ()) {
        parseResultCol ();
        if (hasNext () && is(COMMA)) {
            consume ();
            continue;
        } else {
            break;
        }
    }
}

void Select::parseResultCol () DEF_THROW {
    push ("result-column");

    if (is(STAR)) {
        add ("result-col", next ());
    } else if (has(2) && is(VALUE) && is(1, DOT) && is(2, STAR)) {
        add("result-table", next());
        expect(DOT);
        add("result-col", expect(STAR));
    } else {
        getParser ("EXPRESSION").parse ();
        if (hasNext () && is("AS")) {
            push ("as");
            consume ();
            add ("alias", next());
            pop ();
        } else if (hasNext () && is(VALUE)) {
            add ("alias", next());
        }
    }

    pop ();
}

} }

