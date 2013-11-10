#include "../include/BaseParser.hpp"

namespace Sql {
using namespace Exceptions;

Tokens BaseParser::tokens;

std::map<TOKEN, std::string> Token::Literals = {
    {KEYWORD,       "KEYWORD"},
    {NUMBER,        "NUMBER"}
};

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

template <>
bool BaseParser::is(TOKEN t) const DEF_THROW {
    return tokens.peek ().code == t;
}

template <>
bool BaseParser::is(const char *t) const DEF_THROW {
    return tokens.peek ().value.compare (t) == 0;
}

template <>
bool BaseParser::_is(std::initializer_list<TOKEN> &&ts) const DEF_THROW {
    for (TOKEN t: ts) {
        if (is(t)) {
            return true;
        }
    }
    return false;
}

template <>
bool BaseParser::_is(std::initializer_list<const char *> &&ts) const DEF_THROW {
    for (const char *t: ts) {
        if (is(t)) {
            return true;
        }
    }
    return false;
}

template <>
void BaseParser::expect(TOKEN t) DEF_THROW {
    const SqlToken& token = next();
    if (token.code != t) {
        throw UnexpectedTokenException(token.value, {Token::Literals[t]});
    }
}

template <>
void BaseParser::expect(const char *t) DEF_THROW {
    const SqlToken& token = next();
    if (token.value.compare (t) != 0) {
        throw UnexpectedTokenException(token.value, {t});
    }
}

}
