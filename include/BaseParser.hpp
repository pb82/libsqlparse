#ifndef BASEPARSER_HPP
#define BASEPARSER_HPP

#include <map>
#include <stack>
#include <memory>
#include <iostream>

#include "./Node.hpp"
#include "./Tokens.hpp"
#include "./exceptions/IllegalTokenException.hpp"
#include "./exceptions/UnexpectedTokenException.hpp"
#include "./exceptions/UnknonwSubsetException.hpp"
#include "./exceptions/IllegalParserStateException.hpp"
#include "./exceptions/StreamNotEmptyException.hpp"

#define DEF_THROW throw (                       \
  Exceptions::EndOfStreamException,             \
  Exceptions::IllegalModificationException,     \
  Exceptions::IllegalTokenException,            \
  Exceptions::UnexpectedTokenException,         \
  Exceptions::IllegalParserStateException,      \
  Exceptions::StreamNotEmptyException,          \
  Exceptions::UnknownSubsetException)

#define oneOf(...)  {__VA_ARGS__}

namespace Sql {
namespace Token {
   extern std::map<TOKEN, std::string> Literals;
}

class BaseParser {
public:
    virtual ~BaseParser() = default;
    virtual void parse() DEF_THROW = 0;

    /**
     * @brief getTokenCount return the number of tokens in the stream
     * after a call to feed. Useful to test the tokenizer
     * @return
     */
    unsigned int getTokenCount() const {
        return tokens.getTokenCount ();
    }

    /**
     * Clear all tokens from the stream and reset the parse index
     */
    virtual void reset();
private:
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
     * @brief nodeStack represents the abstract syntax tree
     */
    static std::stack<Node *> nodeStack;

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
     * @brief has returns true if there are at least num tokens left in the
     * stream
     * @return true if enough tokens are left, false otherwise
     */
    inline bool has(unsigned int num)
    const throw (Exceptions::EndOfStreamException) {
        return tokens.has (num);
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
    bool is(std::initializer_list<T>&& t) const DEF_THROW;

    /**
      * returns true if the element is either a value, string literal
      * or a identifier
      */
    bool isName() const DEF_THROW;

    /**
      * returns true if the element at the given index
      * is either a value, string literal or a identifier
      */
    bool isName(unsigned int pos) const DEF_THROW;

    /**
     * is the token at the given index of the stream of the supplied
     * value?
     */
    template <typename T>
    bool is(unsigned int pos, T t) const DEF_THROW;

    /**
      * expect current token to be of a certain type of value
      * throw on unexpected token
      */
    template <typename T>
    const SqlToken& expect(T t) DEF_THROW;

    /**
      * expect current token to be member of a list of supplied
      * values
      * throw on unexpected token
      */
    template <typename T>
    const SqlToken& expect(std::initializer_list<T>&& t) DEF_THROW;

    /**
      * expect the current token to be either a string, value or
      * identifier (something that can be interpreted as a name).
      */
    const SqlToken& expectName() DEF_THROW;

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
    void add(const char *key, const SqlToken& value) {
        nodeStack.top()->appendChild(NodeType::AST_VALUE, key, value.value);
    }

    /**
     * @brief push push a new top level node onto the stack. Subsequent add
     * operations will use the new top level node as reference point
     * @param type
     */
    void push(const char* opcode) {
        Node *const newTopLevel = nodeStack.top()->appendChild(
                        AST_OPCODE,
                        "opcode",
                        opcode);

        nodeStack.push(newTopLevel);
    }

    /**
     * @brief pop remove the current top level node from the stack. This method
     * must be called after a subset parser finished parsing. At the end of
     * the parsing process the stack must contain exactly one node (the root
     * node)
     */
    void pop() {
      nodeStack.pop();
    }

    bool isLiteral() const;
    bool isConstraint() const;

    void parseNameList(const char* name) DEF_THROW;
    void parseExpressionList(const char*name) DEF_THROW;

    /**
     * @brief save preserve parser state for backtracking purposes
     */
    void save() {
        tokens.save ();
    }

    /**
     * @brief restore parser state for backtracking purposes
     */
    void restore() {
        tokens.restore ();
    }
};

}
#endif // BASEPARSER_HPP
