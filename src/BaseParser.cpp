#include "../include/BaseParser.hpp"

using namespace Sql;

Tokens BaseParser::tokens;

/**
 * <Token stream modification methods>
 */

void BaseParser::reset () { tokens.reset (); }

void BaseParser::feed (TOKEN code, const char *value)
    throw (Exceptions::IllegalModificationException) {
    tokens.feed (code, value);
}

const Tokens& BaseParser::getTokens() const {
    return tokens;
}

/**
  * <Token stream query methods>
  */

bool BaseParser::is (TOKEN code) const DEF_THROW {
    return code == tokens.peek ().code;
}

bool BaseParser::is (const char *value) const DEF_THROW {
    return tokens.peek ().value.compare (value) == 0;
}
