#ifndef BASEPARSER_HPP
#define BASEPARSER_HPP

#include <map>

#include "./Node.hpp"
#include "./Tokens.hpp"
#include "./exceptions/IllegalTokenException.h"
#include "./exceptions/UnexpectedTokenException.hpp"

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
private:
    static Tokens tokens;
    static std::map<std::string, BaseParser *> parsers;
protected:
    virtual void parse(Node *const node) DEF_THROW = 0;
    /**
     * <Token stream modification methods>
     */
    void reset();

    void feed(TOKEN code, const char* value)
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
    void expect(T t) DEF_THROW;
};

}
#endif // BASEPARSER_HPP
