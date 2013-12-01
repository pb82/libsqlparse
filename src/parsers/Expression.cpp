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
            if (is("IS")) {
                add("operator", next());
                if (is("NOT")) {
                    add("operator-suffix", next());
                }
            } else {
                add("operator", next());
            }
            parseExpressionPrefix ();
        }
        parseExpressionSuffix ();
        return;
    } else if (is(LP)) {
        add("paren", next ());
        add("paren", expect(RP));
        parseExpressionSuffix ();
    }
}

void Expression::parseExpressionSuffix () DEF_THROW {

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
