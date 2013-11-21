#ifndef BASEPARSER_HPP
#define BASEPARSER_HPP

#include <map>
#include <stack>
#include <memory>

#include "./Node.hpp"
#include "./Tokens.hpp"
#include "./exceptions/IllegalTokenException.h"
#include "./exceptions/UnexpectedTokenException.hpp"
#include "./exceptions/UnknonwSubsetException.h"

#define DEF_THROW throw (                       \
  Exceptions::EndOfStreamException,             \
  Exceptions::IllegalModificationException,     \
  Exceptions::IllegalTokenException,            \
  Exceptions::UnexpectedTokenException)

#define oneOf(...)  _is({__VA_ARGS__})

namespace Sql {
namespace Token {
   extern std::map<TOKEN, std::string> Literals;
}

class BaseParser {
public:
    virtual ~BaseParser() = default;
    virtual void parse() DEF_THROW = 0;
private:
    /**
     * @brief nodeStack represents the abstract syntax tree
     */
    static std::stack<Node *> nodeStack;
    /**
     * @brief tokens The token stream as a static variable, so all the subset
     * parsers access one unique instance
     */
    static Tokens tokens;

    /**
     * @brief parsers A map to store the subset parsers. Static so that all
     * subset parsers access the same instance
     */
    static std::map<std::string, std::unique_ptr<BaseParser>> parsers;
protected:
    /**
     * <Token stream modification methods>
     */
    void reset();

    void feed(TOKEN code, const char* value, unsigned int line)
      throw (Exceptions::IllegalModificationException);

    // Get an immutable reference to the token store
    const Tokens& getTokens() const;

    /**
     * @brief consume increment the stream
     */
    inline void consume() throw (Exceptions::EndOfStreamException) {
        tokens.consume ();
    }

    /**
     * @brief peek return the current token but do not
     * increment the stream
     * @return a reference to the current token in the stream
     */
    inline const SqlToken& peek() const
    throw (Exceptions::EndOfStreamException) {
        return tokens.peek ();
    }

    /**
     * @brief next return the current token and
     * increment the stream
     * @return a reference to the current token on the stream
     */
    inline const SqlToken& next() throw (Exceptions::EndOfStreamException) {
        return tokens.next ();
    }

    /**
     * @brief hasNext returns true if there are tokens left in the stream and
     * a call to 'next' will not throw an exception. Returns false otherwise.
     * @return true if tokens left, false otherwise
     */
    inline bool hasNext() const throw (Exceptions::EndOfStreamException) {
        return tokens.hasNext ();
    }

    /**
      * <Token stream query methods>
      */

    /**
     * is current token of type or value?
     */
    template <typename T>
    bool is(T t) const DEF_THROW;

    /**
     * is current token of one of the types or values in a list?
     */
    template <typename T>
    bool _is(std::initializer_list<T>&& t) const DEF_THROW;

    /**
      * expect current token to be of a certain type of value
      * throw on unexpected token
      */
    template <typename T>
    const SqlToken& expect(T t) DEF_THROW;

    /**
     * @brief registerParser register a parser for a subset of the SQL Syntax
     * @param type Keyword that invokes this parser ('ALTER')
     * @param parser Parser instance
     */
    void registerParser(const char *type, BaseParser *parser) const;

    /**
     * @brief getParse Get a reference to a subset parser for the given type
     * @param type
     * @return
     */
    BaseParser& getParser(const char* type) const
    throw (
        Exceptions::UnknownSubsetException,
        Exceptions::EndOfStreamException);

    /**
     * @brief add add a child nodeto the current top level node but do not
     * increase the level
     * @param type
     */
    void add(NodeType type) {
        nodeStack.top()->appendChild(type);
    }

    void add(NodeType type, std::string&& value) {
        nodeStack.top()->appendChild(type, value);
    }

    void push(NodeType type) {
        Node *const newTopLevel = nodeStack.top()->appendChild(type);
        nodeStack.push(newTopLevel);
    }

    void pop() {
      nodeStack.pop();
    }
};

}
#endif // BASEPARSER_HPP
