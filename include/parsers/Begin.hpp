#ifndef BEGIN_H
#define BEGIN_H

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Begin : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }
#endif // BEGIN_H
