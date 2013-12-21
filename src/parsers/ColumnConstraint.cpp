#include "../../include/parsers/ColumnConstraint.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void ColumnConstraint::parse () DEF_THROW {
    push("col-constraint");

    if(is("CONSTRAINT")) {
        consume ();
        add("constraint-name", expectName ());
    }

    if (is("PRIMARY")) {
        add("constraint-type", expect(KEYWORD));
        expect("KEY");

        if (hasNext () && is(oneOf("ASC", "DESC"))) {
            add("ordering", expect(KEYWORD));
        }

        getParser ("CONFLICT").parse ();

        if (hasNext () && is ("AUTOINCREMENT")) {
            add ("autoincrement", expect(KEYWORD));
        }
    }

    else if (is("NOT")) {
        add("constraint-type", expect(KEYWORD));
        expect("NULL");
        getParser ("CONFLICT").parse ();
    }

    else if (is("UNIQUE")) {
        add("constraint-type", expect(KEYWORD));
        getParser ("CONFLICT").parse ();
    }

    else if (is("CHECK")) {
        add("constraint-type", expect(KEYWORD));
        expect(LP);
        getParser ("EXPRESSION").parse ();
        expect(RP);
    }

    else if (is("DEFAULT")) {
        add("constraint-type", expect(KEYWORD));
        if (isLiteral ()) {
            add("literal", next ());
        } else if (isSignedNumber ()) {
            if (is(PLUS) || is(MINUS)) {
                add("modifier", next());
            }
            add ("number", expect(NUMBER));
        } else {
            expect(LP);
            getParser ("EXPRESSION").parse ();
            expect(RP);
        }
    }

    else if (is("COLLATE")) {
        add("constraint-type", expect(KEYWORD));
        expect(VALUE);
    }

    else if (is("REFERENCES")) {

    }

    else {
        if (hasNext ()) {
            throw UnexpectedTokenException(peek().value, {
                                               "PRIMARY",
                                               "NOT",
                                               "UNIQUE",
                                               "CHECK",
                                               "DEFAULT",
                                               "REFERENCES",
                                               "COLLATE"}, peek().line);
        } else {
            throw EndOfStreamException();
        }
    }

    pop();
}

bool ColumnConstraint::isSignedNumber () const {
    if (!hasNext ()) {
        return false;
    }

    if (is(NUMBER)) {
        return true;
    }

    if (has(1) && (is(PLUS) || is(MINUS)) && is(1, NUMBER)) {
        return true;
    }

    return false;
}

} }

