
#include "../../include/parsers/TypeName.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void TypeName::parse () DEF_THROW {
    push("typename");

    while (hasNext () && is(VALUE)) {
        add ("type-name", next());
    }

    if (hasNext () && is(LP)) {
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
