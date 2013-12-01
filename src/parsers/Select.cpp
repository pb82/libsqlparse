#include "../../include/parsers/Select.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Select::parse () DEF_THROW {
    push("select-stmt");
    expect("SELECT");
    if (is(oneOf("DISTINCT", "ALL"))) {
        add("modifier", expect(oneOf("DISTINCT", "ALL")));
    }

    do {
        parseResultCol ();
    } while (hasNext () && is(COMMA));

    pop();
}

void Select::parseResultCol () DEF_THROW {
    if (is(STAR)) {
        add ("result-col", next ());
        return;
    } else if (has(2) && is(VALUE) && is(1, DOT) && is(2, STAR)) {
        add("result-table", next());
        expect(DOT);
        expect(STAR);
        return;
    } else {
        getParser ("EXPRESSION").parse ();
        if (is("AS")) {
            consume ();
            add ("alias", next());
        } else if (is(VALUE)) {
            add ("alias", next());
        }

        return;
    }
}

} }

