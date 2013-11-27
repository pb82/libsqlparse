
#include "../../include/parsers/TypeName.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void TypeName::parse () DEF_THROW {
    push("typename");

    while (is(VALUE)) {
        add ("type-name", next());
    }

    if (is(LP)) {
        consume ();
        add ("leading", expect(NUMBER));

        if (is(COMMA)) {
            consume ();
            add("trailing", expect(NUMBER));
        }
        expect(RP);
    }

    pop();
}

} }
