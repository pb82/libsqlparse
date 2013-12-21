#include "../../include/parsers/AlterTable.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void AlterTable::parse () DEF_THROW {
    push("alter");

    expect("ALTER");
    expect("TABLE");

    if (has(2) && is(1, DOT) && is (2, VALUE)) {
        add("database-name", expect(VALUE));
        consume();
        add("table-name", expect(VALUE));
    } else {
        add("table-name", expect(VALUE));
    }

    if (is("RENAME")) {
        push("rename");
        consume();
        expect("TO");
        add("column-name", expect(VALUE));
        pop();
    } else if (is("ADD")) {
        push("add");
        consume();
        if (is("COLUMN")) {
            consume();
        }
        getParser ("COLDEF").parse ();
        pop();
    } else {
        throw UnexpectedTokenException(
            peek().value,
            {"RENAME", "ADD"},
            peek().line);
    }

    pop();
}

} }
