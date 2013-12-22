#include "../../include/parsers/Analyze.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Analyze::parse () DEF_THROW {
    push("analyze");
    expect("ANALYZE");

    if (has(2) && is(0, VALUE) && is(1, DOT) && is(2, VALUE)) {
        add("database-name", expect(VALUE));
        expect(DOT);
        add("table-or-index-name", expect(VALUE));
    }

    if (hasNext () && is(VALUE)) {
        add("name", next());
    }

    pop();
}

} }
