#include "../../include/parsers/Expression.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Expression::parse () DEF_THROW {
    parseExpressionPrefix ();
}

void Expression::parseExpressionPrefix () DEF_THROW {
    push("expression");

    if(is(oneOf("-", "+", "~", "NOT"))) {
        add("unary-operator",
            expect(
                oneOf(
                    "-",
                    "+",
                    "~",
                    "NOT")));
    }

    parseExpressionCore ();
    pop();
}

void Expression::parseExpressionCore () DEF_THROW {
    if (isLiteral ()) {
        add("literal", next ());
        if (isCombinator ()) {
            parseCombinator ();
            parseExpressionPrefix ();
        }
        parseExpressionSuffix ();
        return;
    } else if (is(LP)) {
        add("paren", next ());
        if (is("SELECT")) {
            getParser ("SELECT").parse ();
            add("paren", expect(RP));
            parseExpressionSuffix ();
            return;
        }

        parseExpressionPrefix ();
        if (isCombinator ()) {
            parseCombinator ();
            parseExpressionPrefix ();
        }

        add("paren", expect(RP));
        parseExpressionSuffix ();
        return;
    } else if (is(PARAMETER)) {
        add("parameter", next());
        parseExpressionSuffix ();
        return;
    } else if (is(VALUE)) {
        if (has(4) && is(1, DOT) && is (2,VALUE) && is(3, DOT)
                && is(4, VALUE)) {
            add("database-name", next());
            expect(DOT);
            add("table-name", next());
            expect(DOT);
            add("column-name", next());
            parseExpressionSuffix ();
            return;
        } else if (has(3) && is(1, DOT) && is (2, VALUE)) {
            add("table-name", next());
            expect(DOT);
            add("column-name", next());
            parseExpressionSuffix ();
            return;
        } else if (has(1) && is(1, LP)) {
            add("function-name", next());
            add("paren", next());
            if (is(RP)) {
                add("paren", next());
                parseExpressionSuffix ();
                return;
            } else if (is(STAR)) {
                add("star", next ());
                add("paren", expect(RP));
                parseExpressionSuffix ();
                return;
            } else if (is("DISTINCT")) {
                add ("modifier", next());
                parseExpressionList ();
                add("paren", expect(RP));
                parseExpressionSuffix ();
                return;
            } else {
                parseExpressionList ();
                add("paren", expect(RP));
                parseExpressionSuffix ();
                return;
            }
        } else {
            add("column-name", next ());
            parseExpressionSuffix ();
            return;
        }
        return;
    }
}

void Expression::parseExpressionSuffix () DEF_THROW {
    if (isCombinator ()) {
        parseCombinator ();
        parseExpressionPrefix ();
    }
}

void Expression::parseExpressionList () DEF_THROW {
    do {
        if (is(COMMA)) consume ();
        push("parameter");
        parseExpressionPrefix ();
        pop();
    } while (is(COMMA));
}

void Expression::parseCombinator () DEF_THROW {
    if (is("IS")) {
        add("operator", next());
        if (is("NOT")) {
            add("operator-suffix", next());
        }
    } else {
        add("operator", next());
    }
}

bool Expression::isLiteral () const {
    if (!hasNext ()) {
        return false;
    }

    return
        is(NUMBER)          ||
        is(STRING)          ||
        is(BLOB)            ||
        is("NULL")          ||
        is("CURRENT_DATE")  ||
        is("CURRENT_TIME")  ||
        is("CURRENT_TIMESTAMP");
}

bool Expression::isCombinator () const {
    if (!hasNext ()) {
        return false;
    }

    if(is (
            oneOf(
                PLUS,
                MINUS,
                SLASH,
                STAR,
                REM,
                CONCAT,
                BITAND,
                BITOR,
                LSHIFT,
                RSHIFT,
                LE,
                LT,
                GE,
                GT,
                EQ,
                NE))) return true;

    if (is (
            oneOf(
                "IS",
                "IN",
                "LIKE",
                "GLOB",
                "MATCH",
                "REGEXP",
                "AND",
                "OR"
                    ))) return true;

    return false;
}

} }
