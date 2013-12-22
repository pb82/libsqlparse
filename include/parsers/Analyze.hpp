#ifndef ANALYZE_HPP
#define ANALYZE_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Analyze : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // ANALYZE_HPP
