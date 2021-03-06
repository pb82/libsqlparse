#include "../include/Parser.hpp"

namespace Sql {
using namespace Exceptions;
using namespace Parsers;

void Parser::parse() DEF_THROW {
    registerSubsets();
    nodeStack.push (&root);

    if (is("EXPLAIN")) {
      consume();
      if (is("QUERY")) {
        consume();
        expect("PLAN");
      }
    }

    getParser(peek().value.c_str()).parse();
    // Eat trailing semicolon
    if (hasNext () && is(SEMI)) {
        consume ();
    }

    if (1 != BaseParser::nodeStack.size()) {
      throw IllegalParserStateException();
    }

    if (BaseParser::hasNext ()) {
        throw StreamNotEmptyException();
    }
}

void Parser::registerSubsets() const {
  registerParser ("ALTER",          new AlterTable);
  registerParser ("COLDEF",         new ColumnDef);
  registerParser ("TYPENAME",       new TypeName);
  registerParser ("COLCONSTR",      new ColumnConstraint);
  registerParser ("CONFLICT",       new ConflictClause);
  registerParser ("EXPRESSION",     new Expression);
  registerParser ("SELECT",         new Select);
  registerParser ("REFERENCES",     new ForeignKeyClause);
  registerParser ("ANALYZE",        new Analyze);
  registerParser ("ATTACH",         new Attach);
  registerParser ("BEGIN",          new Begin);
  registerParser ("COMMIT",         new Commit);
  registerParser ("END",            new Commit);
  registerParser ("ROLLBACK",       new Rollback);
  registerParser ("CREATE",         new Create);
  registerParser ("UPDATE",         new Update);
  registerParser ("INSERT",         new Insert);
  registerParser ("DELETE",         new Delete);
  registerParser ("QUALIFIEDTABLE", new QualifiedTableName);
  registerParser ("ORDERINGTERM",   new OrderingTerm);
}

void Parser::feed (std::stringstream &source)
    throw (IllegalModificationException, IllegalTokenException) {
    feed(source.str ().c_str ());
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

void Parser::reset () {
    BaseParser::reset ();
    root = Node();
}

void Parser::printTokensToStream(std::ostream &stream) const {
    stream << getTokens ();
}

void Parser::printSyntaxTree(std::ostream &stream) const {
    printSyntaxTreeInternal(stream, &root);
}

void Parser::printSyntaxTreeInternal(std::ostream &stream,
    const Node *node) const {

    static int indent = 0;
    indent ++;

    auto printIndentation = [] (int num, std::ostream &stream) {
        for (int i = 0; i < num; i++) {
          stream << "..";
        }
    };

    printIndentation(indent, stream);
    stream
            << "[(" << node->getCode()
            << ") " << node->getKey ()
            << ": " << node->getValue()
            << "]"  << std::endl;

    if (node->getChildren().size() > 0) {
        for (const Node* child: node->getChildren()) {
          printSyntaxTreeInternal(stream, child);
        }
    }

    indent--;
}

}
