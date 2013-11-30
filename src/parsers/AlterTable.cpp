#include "../../include/parsers/AlterTable.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void AlterTable::parse () DEF_THROW {
    push("alter");

    expect("ALTER");
    expect("TABLE");

    add("target-arg1", expect(VALUE));

    if (is(DOT)) {
        consume();
        add("target-arg2", expect(VALUE));
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
