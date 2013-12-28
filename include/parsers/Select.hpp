#ifndef SELECT_H
#define SELECT_H

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Select : public BaseParser {
public:
    void parse() DEF_THROW;
private:
    void parseResultCol() DEF_THROW;
    void parseResultColList() DEF_THROW;
    void parseJoinSource() DEF_THROW;
    void parseSingleSource() DEF_THROW;
    void parseJoinOp() DEF_THROW;
    void parseJoinOpList() DEF_THROW;
    void parseJoinConstraint() DEF_THROW;

    bool isJoinOp() const;
};

} }

#endif // SELECT_H
