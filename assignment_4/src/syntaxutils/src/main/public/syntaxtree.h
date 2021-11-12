/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAXUTILS_SYNTAXTREE
#define COCO_FRAMEWORK_SYNTAXUTILS_SYNTAXTREE

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "node.h"

class SyntaxTree {
    public:
    SyntaxTree() = default;
    virtual ~SyntaxTree();

    Node* getRoot(size_t id) const;

    // creates an unary parent node
    static Node* createParentNode(NodeType nodeType, ReturnType returnType, Node* child);

    // creates a binary parent node
    static Node* createParentNode(NodeType nodeType, ReturnType ret, Node* leftChild, Node* rightChild);

    // creates an integer leaf with node type NODE_NUM
    template<typename T>
    static Node* createNumNode(T value, ReturnType rt) {
        auto* node = new ConstantNode<T>;
        node->setNodeType(NODE_NUM);
        node->setReturnType(rt);
        node->setValue(value);

        return node;
    }

    // creates a symbol leaf with node type NODE_ID
    static Node* createIdNode(size_t id, ReturnType rt);

    // creates a leaf with node type NODE_EMPTY
    static Node* createLeaf();

    inline friend std::ostream& operator<<(std::ostream& stream, const SyntaxTree& tree) {
        return tree.doStream(stream, 4, nullptr);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, int indent, const SymbolTable* table) const {
        stream << "===== Syntax tree dump =====\n";
        for (auto& function: functions) {
            stream << "Function: " << function.second.name << '\n';
            function.second.root->doStream(stream, indent, indent, table);
        }
        stream << "===== End of syntax tree dump =====\n";
        return stream;
    }

    private:
    friend class FrontendBuilder;

    struct FunctionInfo {
        std::string name;
        Node* root = nullptr;
    };

    // Map from function-id to its root
    std::unordered_map<size_t, FunctionInfo> functions;
};

#endif
