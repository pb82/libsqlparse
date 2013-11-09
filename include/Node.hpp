#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

#include "./Token.hpp"

struct Node {
public:
  Node()
    : code(ROOT),
      value("ROOT") {
  }

  ~Node() {
    for (const Node *child: children) {
      delete child;
    }
  }

  Node* appendChild(TOKEN code) {
    return appendChild(code, "");
  }

  Node* appendChild(TOKEN code, const char* value) {
    Node *newNode = new Node(code, value);
    this->children.push_back(newNode);
    return newNode;
  }

  /**
   * @brief getCode
   * @return the token code
   */
  TOKEN getCode() const {
    return code;
  }

  /**
   * @brief getValue
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

#endif // NODE_HPP
