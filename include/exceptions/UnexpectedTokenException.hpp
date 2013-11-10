#ifndef UNEXPECTEDTOKENEXCEPTION_HPP
#define UNEXPECTEDTOKENEXCEPTION_HPP

#include <string>
#include <vector>

namespace Sql { namespace Exceptions {

class UnexpectedTokenException : public std::runtime_error {
public:
    UnexpectedTokenException(
        std::string found,
        std::initializer_list<std::string> expected)
        : std::runtime_error ("Unexpected token"),
          found(found),
          expected(expected) { }

    const std::string& getFound() const { return found; }
    const std::vector<std::string>& getExpected() const {
        return expected;
    }
private:
    /**
     * @brief found the token that was found in the stream
     * when a different one was expected
     */
    std::string found;

    /**
     * @brief expected a list of expected tokens (or values thereof)
     * TODO: accept token codes here?
     */
    std::vector<std::string> expected;
};


} }

#endif // UNEXPECTEDTOKENEXCEPTION_HPP
