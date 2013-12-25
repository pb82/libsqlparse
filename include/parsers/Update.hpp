#ifndef UPDATE_HPP
#define UPDATE_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Update : public BaseParser {
public:
    void parse() DEF_THROW;

private:
    void parseUpdateColumns() DEF_THROW;
};

} }

#endif // UPDATE_HPP
