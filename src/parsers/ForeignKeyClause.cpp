#include "../../include/parsers/ForeignKeyClause.hpp"

namespace Sql { namespace Parsers {
using namespace Exceptions;

void ForeignKeyClause::parse () DEF_THROW {
    push("references");

    pop();
}

} }


