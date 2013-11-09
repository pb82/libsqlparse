#ifndef BASEPARSER_HPP
#define BASEPARSER_HPP

#include <vector>

#include "./Node.hpp"
#include "./Tokens.hpp"

namespace Sql {

class BaseParser {
protected:
    static Tokens tokens;
};

}
#endif // BASEPARSER_HPP
