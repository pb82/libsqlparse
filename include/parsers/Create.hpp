#ifndef CREATE_HPP
#define CREATE_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Create : public BaseParser {
public:
    void parse() DEF_THROW;

private:
    void parseCreateIndex() DEF_THROW;
    void parseColumnList() DEF_THROW;
    void parseIndexedColumn() DEF_THROW;
};

} }

#endif // CREATE_HPP
