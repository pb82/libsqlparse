#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

namespace Sql {

enum NodeType {
  ROOT,
  NODE_VALUE,
  ALTER_TABLE,
  RENAME_TO
};

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
    for (Node *child: children) {
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
  Node *appendChild(NodeType code) {
    return appendChild(code, "");
  }

  /**
   * @brief appendChild append a new node defined by it's code
   * and value (e.g. Sql Keywords, Strings...)
   * @param code Token Code (Keyword, String...)
   * @param value (actual Token value)
   * @return a pointer to the newly inserted node
   */
  Node *appendChild(NodeType code, const char* value) {
      children.push_back (new Node(code, value));
      return children.at (children.size () -1);
  }

  /**
   * @brief appendChild same as above but takes a string reference
   * @param code
   * @param value
   * @return
   */
  Node *appendChild(NodeType code, const std::string& value) {
    return appendChild(code, value.c_str());
  }

  /**
   * @brief getCode return this node's code (type)
   * @return the token code
   */
  NodeType getCode() const {
    return code;
  }

  /**
   * @brief getValue return this node's value
   * @return a copy of the token value
   */
  std::string getValue() const {
    return value;
  }

  /**
   * @brief getChildren returns an immutable reference to the
   * children array
   * @return
   */
  const std::vector<Node *>& getChildren() const {
    return this->children;
  }

private:
  /**
   * @brief Node
   * Private constructor. Only root node creation is permitted.
   * Child nodes are created by using the appendChild methods.
   * @param code
   * @param value
   */
  Node(NodeType code, const char* value) {
    this->code = code;
    this->value = value;
  }

  NodeType code;
  std::string value;
  std::vector<Node *> children;
};

}
#endif // NODE_HPP
