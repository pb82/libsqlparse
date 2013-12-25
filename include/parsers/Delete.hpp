#ifndef DELETE_HPP
#define DELETE_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Delete : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // DELETE_HPP
