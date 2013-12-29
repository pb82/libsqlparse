#ifndef ORDERINGTERM_HPP
#define ORDERINGTERM_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class OrderingTerm : public BaseParser {
public:
    void parse() DEF_THROW;

private:
    void parseOrderBy() DEF_THROW;
    void parseLimit() DEF_THROW;
    void parseTermList() DEF_THROW;
    void parseTerm() DEF_THROW;


    bool isOrderingTerm() const;
};

} }

#endif // ORDERINGTERM_HPP
