#include "../include/Parser.hpp"

void Parser::parse(const char *source) DEF_THROW {
  tokens.reset();
  feedTokens(source);

  return;
}

void Parser::feedTokens(const char *source)
  throw (IllegalModificationException, IllegalTokenException) {
  TOKEN current_token;
  std::istringstream ss(source);
  yyFlexLexer lexer(&ss, 0);

  while ((current_token = lexer.yylex()) != 0) {
    if (ERROR == current_token) {
      throw IllegalTokenException(lexer.YYText());
    }
    tokens.feed(current_token, lexer.YYText());
  }
}

void Parser::printTokensToStream(std::ostream &stream) {
  stream << tokens;
}
