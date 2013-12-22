#ifndef COMMIT_HPP
#define COMMIT_HPP

#include "../BaseParser.hpp"

namespace Sql { namespace Parsers {

class Commit : public BaseParser {
public:
    void parse() DEF_THROW;
};

} }

#endif // COMMIT_HPP
