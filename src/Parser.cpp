#include "../include/Parser.hpp"

namespace Sql {
using namespace Exceptions;
using namespace Parsers;


void Parser::parse() DEF_THROW {
    registerSubsets();
    push(NodeType::ROOT);

    if (is("EXPLAIN")) {
      consume();
      if (is("QUERY")) {
        consume();
        expect("PLAN");
      }
    }

    getParser(peek().value.c_str()).parse();
}

void Parser::registerSubsets() const {
  registerParser("ALTER", new AlterTable);
}

void Parser::feed(const char *source)
  throw (IllegalModificationException, IllegalTokenException) {
  reset ();

  TOKEN current_token;
  std::istringstream ss(source);
  yyFlexLexer lexer(&ss, 0);

  int current_line = 1;
  while ((current_token = lexer.yylex()) != 0) {
    if (ERROR == current_token) {
        // Tokenizer encountered invalid token
        throw IllegalTokenException(lexer.YYText(), current_line);
    } else if (KEYWORD == current_token) {
        // Save keyword in capital form in order to be able
        // to compare them easily
        std::string keyword = lexer.YYText ();
        std::transform(
                    keyword.begin (),
                    keyword.end (),
                    keyword.begin (),
                    toupper);
        BaseParser::feed (current_token, keyword.c_str (), current_line);
    } else if (NEWLINE == current_token) {
        current_line++;
    } else {
        // Store all other tokens normally with code
        // and value
        BaseParser::feed (current_token, lexer.YYText(), current_line);
    }
  }
}

void Parser::printTokensToStream(std::ostream &stream) const {
  stream << getTokens ();
}

}
