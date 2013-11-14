#include "../include/BaseParser.hpp"

namespace Sql {
using namespace Exceptions;

// Static initialization for the token stream
Tokens BaseParser::tokens;

// Static initialization for the fragment parsers map
std::map<std::string, std::unique_ptr<BaseParser>> BaseParser::parsers;

std::map<TOKEN, std::string> Token::Literals = {
    {KEYWORD,       "KEYWORD"},
    {NUMBER,        "NUMBER"},
    {BLOB,          "BLOB"},
    {VALUE,         "VALUE"},
    {PARAMETER,     "PARAMETER"},
    {STRING,        "STRING"}
};

/**
 * <Token stream modification methods>
 */

void BaseParser::reset () { tokens.reset (); }

void BaseParser::feed (TOKEN code, const char *value, unsigned int line)
    throw (Exceptions::IllegalModificationException) {
    tokens.feed (code, value, line);
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
const SqlToken& BaseParser::expect(TOKEN t) DEF_THROW {
    const SqlToken& token = next();
    if (token.code != t) {
        throw UnexpectedTokenException(
            token.value,
            {Token::Literals[t]},
            token.line);
    }
    return token;
}

template <>
const SqlToken& BaseParser::expect(const char *t) DEF_THROW {
    const SqlToken& token = next();
    if (token.value.compare (t) != 0) {
        throw UnexpectedTokenException(token.value, {t}, token.line);
    }
    return token;
}

void BaseParser::registerParser(const char *type, BaseParser *parser) const {
  parsers[type] = std::unique_ptr<BaseParser>(parser);
}

BaseParser& BaseParser::getParser(const char *type) const
throw (UnknownSubsetException, EndOfStreamException) {
  if (parsers.find(type) == parsers.end()) {
    throw UnknownSubsetException(type, peek().line);
  }
  return *(parsers[type]);
}

}
