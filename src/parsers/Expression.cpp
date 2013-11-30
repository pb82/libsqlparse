#include "../../include/parsers/Expression.h"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void Expression::parse () DEF_THROW {
    push("expression");
    pop();
}

} }
