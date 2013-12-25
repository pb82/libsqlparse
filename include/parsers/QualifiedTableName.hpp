#ifndef QUALIFIEDTABLENAME_HPP
#define QUALIFIEDTABLENAME_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class QualifiedTableName : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // QUALIFIEDTABLENAME_HPP
