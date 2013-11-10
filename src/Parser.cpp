#include "../include/Parser.hpp"

namespace Sql {
using namespace Exceptions;

void Parser::parse(const char *source) DEF_THROW {
  reset ();
  feedTokens(source);
}

void Parser::feedTokens(const char *source)
  throw (IllegalModificationException, IllegalTokenException) {
  TOKEN current_token;
  std::istringstream ss(source);
  yyFlexLexer lexer(&ss, 0);

  while ((current_token = lexer.yylex()) != 0) {
    if (ERROR == current_token) {
        // Tokenizer encountered invalid token
        throw IllegalTokenException(lexer.YYText());
    } else if (KEYWORD == current_token) {
        // Save keyword in capital form in order to be able
        // to compare them easily
        std::string keyword = lexer.YYText ();
        std::transform(
                    keyword.begin (),
                    keyword.end (),
                    keyword.begin (),
                    toupper);
        feed (current_token, keyword.c_str ());
    } else {
        // Store all other tokens normally with code
        // and value
        feed (current_token, lexer.YYText());
    }
  }
}

void Parser::printTokensToStream(std::ostream &stream) const {
  stream << getTokens ();
}

}
