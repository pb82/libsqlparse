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
        parseExpressionSuffix ();
        return;
    } else if (is(LP)) {
        between.push (false);
        add("paren", next ());
        if (is("SELECT")) {
            getParser ("SELECT").parse ();
            expect(RP);
            parseExpressionSuffix ();
            return;
        }

        parseExpressionPrefix ();
        if (isCombinator ()) {
            parseCombinator ();
            parseExpressionPrefix ();
        }

        add("paren", expect(RP));
        between.pop ();

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
        } else if (has(2) && is(1, DOT) && is (2, VALUE)) {
            add("table-name", next());
            expect(DOT);
            add("column-name", next());
            parseExpressionSuffix ();
            return;
        } else if (has(1) && is(1, LP)) {
            add("function-name", next());
            consume();
            if (is(RP)) {
                consume ();
                parseExpressionSuffix ();
                return;
            } else if (is(STAR)) {
                add("star", next ());
                expect(RP);
                parseExpressionSuffix ();
                return;
            } else if (is("DISTINCT")) {
                add ("modifier", next());
                parseExpressionList ();
                expect(RP);
                parseExpressionSuffix ();
                return;
            } else {
                parseExpressionList ();
                expect(RP);
                parseExpressionSuffix ();
                return;
            }
        } else {
            add("column-name", next ());
            parseExpressionSuffix ();
            return;
        }
    } else if (is("CAST")) {
        consume ();
        push("cast");
        expect(LP);
        parseExpressionPrefix ();
        expect("AS");
        getParser ("TYPENAME").parse ();
        expect(RP);
        pop();
        parseExpressionSuffix ();
        return;
    } else if (is(oneOf("NOT", "EXISTS"))) {
        const SqlToken& token = expect(oneOf("NOT", "EXISTS"));
        push ("exists");
        if (token.value.compare ("NOT") == 0) {
            add ("modifier", token);
            expect ("EXISTS");
        }

        expect("(");
        getParser ("SELECT").parse ();
        expect(")");
        pop();

        parseExpressionSuffix ();
        return;
    } else if (is("CASE")) {
        consume();
        push("case");

        if (!is("WHEN")) {
            parseExpressionPrefix ();
        }

        parseWhenList ();

        const SqlToken& token = expect(oneOf("ELSE","END"));
        if (token.value.compare ("ELSE") == 0) {
            parseExpressionPrefix ();
            expect ("END");
        }

        pop ();

        parseExpressionSuffix ();
        return;
    } else if (is("RAISE")) {
        consume();
        push("raise");

        expect(LP);
        const SqlToken& token = expect(
                    oneOf("IGNORE", "ROLLBACK", "ABORT", "FAIL"));

        if (token.value.compare ("IGNORE") == 0) {
            add("action", token);
        } else {
            add("action", token);
            expect(COMMA);
            add("error-msg", expect(STRING));
        }

        expect(RP);

        pop();
        parseExpressionSuffix ();
    }
}

void Expression::parseExpressionSuffix () DEF_THROW {
    if (isCombinator ()) {
        parseCombinator ();
        parseExpressionPrefix ();
    }

    if (is(oneOf("ISNULL", "NOTNULL"))) {
        add("postfix", next());
        return;
    }

    else if (is("IS")) {
        add("compare-to", next());
        if (is ("NOT")) {
            add("modifier", next());
        }
        parseExpressionPrefix ();
        return;
    }

    else if (is("BETWEEN")) {
        parseBetween ();
        return;
    }

    else if (is("IN")) {
        parseInStatement ();
        return;
    }

    else if (is(oneOf("LIKE", "GLOB", "REGEXP", "MATCH"))) {
        parseComparison ();
        return;
    }

    else if (is("COLLATE")) {
        consume();
        add("collate", expect(VALUE));
    }

    else if (is("NOT")) {
        add("postfix", next());
        if (is("NULL")) {
            add("comparator", next());
            return;
        }

        else if (is("BETWEEN")) {
            parseBetween ();
            return;
        }

        else if (is(oneOf("LIKE", "GLOB", "REGEXP", "MATCH"))) {
            parseComparison ();
            return;
        }

        else if (is("IN")) {
            parseInStatement ();
            return;
        }

        else {
            if (hasNext ()) {
                throw UnexpectedTokenException(peek().value, {
                                                   "NULL",
                                                   "BETWEEN",
                                                   "LIKE",
                                                   "GLOB",
                                                   "REGEXP",
                                                   "MATCH"}, peek().line);
            } else {
                throw EndOfStreamException();
            }
        }
    }
}

void Expression::parseInStatement () DEF_THROW {
    add("in", expect("IN"));
    if (is(LP)) {
        consume ();
        if (is(RP)) {
            consume();
            return;
        }

        else if (is("SELECT")) {
            getParser ("SELECT").parse ();
            expect(RP);
            return;
        }

        else {
            parseExpressionList ();
            expect(RP);
            return;
        }
    } else {
        if (has(2) && is(1, DOT) && is(2, VALUE)) {
            add("database-name", next());
            consume ();
            add("table-name", next());
            return;
        } else {
            add("table-name", expect(VALUE));
            return;
        }
    }
}

void Expression::parseComparison () DEF_THROW {
    add("comparator", expect(oneOf("LIKE", "GLOB", "REGEXP", "MATCH")));
    parseExpressionPrefix ();
    if (is("ESCAPE")) {
        add ("escape", next());
        parseExpressionPrefix ();
    }
    return;
}

void Expression::parseBetween () DEF_THROW {
    between.push (true);
    add("between", expect("BETWEEN"));
    parseExpressionPrefix ();
    add("and", expect("AND"));
    parseExpressionPrefix ();
    between.pop ();
    return;
}

void Expression::parseWhenList () DEF_THROW {
    do {
        push("when");
        expect("WHEN");
        parseExpressionPrefix ();
        expect("THEN");
        parseExpressionPrefix ();
        pop ();
    } while (is("WHEN"));
}

void Expression::parseExpressionList () DEF_THROW {
    while (hasNext ()) {
        push ("parameter");
        parseExpressionPrefix ();
        pop ();
        if (hasNext () && is(COMMA)) {
            consume();
            continue;
        } else {
            break;
        }
    }
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

bool Expression::inBetween () const {
    if (between.size () == 0) {
        return false;
    }

    return between.top ();
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
                // 'IN' should only ever be used in the form
                // 'Id IN (1,2,3) right?'
                /*"IN",*/
                "OR"
                    ))) return true;

    if (!inBetween () && is("AND")) {
        return true;
    }

    return false;
}

} }
