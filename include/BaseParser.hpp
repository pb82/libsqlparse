#ifndef BASEPARSER_HPP
#define BASEPARSER_HPP

#include <vector>
#include <cstdarg>

#include "./Node.hpp"
#include "./Tokens.hpp"
#include "./exceptions/IllegalTokenException.h"

#define DEF_THROW throw (                       \
  Exceptions::EndOfStreamException,             \
  Exceptions::IllegalModificationException,     \
  Exceptions::IllegalTokenException)

namespace Sql {

class BaseParser {
public:
    virtual ~BaseParser() = default;
private:
    static Tokens tokens;
protected:
    /**
     * <Token stream modification methods>
     */
    void reset();
    void feed(TOKEN code, const char* value)
      throw (Exceptions::IllegalModificationException);

    // Get an immutable reference to the token store
    const Tokens& getTokens() const;

    /**
      * <Token stream query methods>
      */
    bool is(TOKEN code) const DEF_THROW;
    bool is(const char *value) const DEF_THROW;
};

}
#endif // BASEPARSER_HPP
