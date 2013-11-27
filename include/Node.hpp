#ifndef NODE_HPP
#define NODE_HPP

#include <map>
#include <vector>
#include <string>

namespace Sql {

enum NodeType {
  AST_ROOT,
  AST_OPCODE,
  AST_VALUE
};

struct Node {
public:
    /**
      * @brief Node public constructor allows only
      * root node creation
      */
    Node()
    : code(AST_ROOT),
      key("ROOT"),
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
      * @brief appendChild append a new node defined by it's code
      * and value (e.g. Sql Keywords, Strings...)
      * @param code Token Code (Keyword, String...)
      * @param value (actual Token value)
      * @return a pointer to the newly inserted node
      */
    Node *appendChild(NodeType code, const char* key, const char* value) {
        children.push_back (new Node(code, key, value));
        return children.at (children.size () -1);
    }

    /**
      * @brief appendChild same as above but takes a string reference
      * @param code
      * @param value
      * @return
      */
    Node *appendChild(
        NodeType code,
        const char* key,
        const std::string& value) {
        return appendChild(code, key, value.c_str());
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
     * @brief getKey return a copy of the param key of this node
     * @return a std::string containing the param key
     */
    std::string getKey() const {
        return key;
    }

    /**
     * @brief getParam return a copy to an entry of a parameter
     * @param key parameter name
     * @return a copy to the string value of the parameter
     */
    std::string getParam(std::string&& key) const {
        for (const Node *node: children) {
            if (key.compare (node->key) == 0) {
                return node->value;
            }
        }
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
    Node(NodeType code, const char* key, const char* value) {
        this->code = code;
        this->key = key;
        this->value = value;
    }

    NodeType code;
    std::string key;
    std::string value;
    std::vector<Node *> children;
};

}
#endif // NODE_HPP
