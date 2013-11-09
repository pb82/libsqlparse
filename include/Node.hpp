#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

#include "./Token.hpp"

namespace Sql {

struct Node {
public:
  /**
   * @brief Node public constructor allows only
   * root node creation
   */
  Node()
    : code(ROOT),
      value("ROOT") {
  }

   /**
    * Destroy child nodes
    */
  ~Node() {
    for (const Node *child: children) {
      delete child;
    }
  }

  /**
   * @brief appendChild append a new child node without
   * a value (only a token code). This method can be used
   * to append tokens like operators which are defined by
   * their codes and are not dependent on a (string) value.
   * @param code token code
   * @return a pointer to the newly inserted node
   */
  const Node* appendChild(TOKEN code) {
    return appendChild(code, "");
  }

  /**
   * @brief appendChild append a new node defined by it's code
   * and value (e.g. Sql Keywords, Strings...)
   * @param code Token Code (Keyword, String...)
   * @param value (actual Token value)
   * @return a pointer to the newly inserted node
   */
  const Node* appendChild(TOKEN code, const char* value) {
      children.push_back (new Node(code, value));
      return children.at (children.size () -1);
  }

  /**
   * @brief getCode return this node's code (type)
   * @return the token code
   */
  TOKEN getCode() const {
    return code;
  }

  /**
   * @brief getValue return this node's value
   * @return a copy of the token value
   */
  std::string getValue() const {
    return value;
  }

private:
  /**
   * @brief Node
   * Private constructor. Only root node creation is permitted.
   * Child nodes are created by using the appendChild methods.
   * @param code
   * @param value
   */
  Node(TOKEN code, const char* value) {
    this->code = code;
    this->value = value;
  }

  TOKEN code;
  std::string value;
  std::vector<const Node *> children;
};

}
#endif // NODE_HPP
