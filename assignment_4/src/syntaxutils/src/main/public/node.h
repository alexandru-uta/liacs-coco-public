/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAXUTILS_NODE
#define COCO_FRAMEWORK_SYNTAXUTILS_NODE

#include <limits>
#include <functional>
#include <typeinfo>

#include "symbol.h"
#include "symboltable.h"
#include "types.h"

#include <to_string.h>
#include <hacks/template_hacks.h>


class SymbolTable;

class Node {
    public:
    Node() : nodeType(NODE_UNKNOWN), returnType(RT_UNKNOWN) {}
    Node(NodeType nodeType, ReturnType returnType) : nodeType(nodeType), returnType(returnType) {}

    virtual ~Node() = default;

    NodeType getNodeType() const;
    void setNodeType(NodeType nodeType);

    ReturnType getReturnType() const;
    void setReturnType(ReturnType type);

    inline friend std::ostream& operator<<(std::ostream& stream, const Node& node) {
        return node.doStream(stream);
    }

    inline virtual std::ostream& doStream(std::ostream& stream) const {
        return doStream(stream, 0, 4);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, int indent, int indent_add) const {
        return doStream(stream, indent, indent_add, nullptr);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, int indent, int /*indent_add*/, const SymbolTable* /*table*/) const {
        stream << std::left << std::setw(indent) << "" << util::to_string(returnType) << " " << util::to_string(nodeType);
        if (nodeType == NODE_EMPTY)
            stream << '\n';
        return stream;
    }

    inline bool operator==(const Node& other) const {
        return equal_to(other, nullptr, nullptr);
    }

    /**
     * Checks equivalence with `other`.
     * @param other Node to compare with.
     * @param ltable Symboltable for LHS expression ('this' node subtree).
     * @param rtable Symboltable for RHS expression (other node subtree).
     * @note If any of ltable, rtable is a `nullptr`, does not compare symbols.
     * @return `true` iff equal, `false` otherwise.
     */
    inline bool equal_to(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const {
        return equals(other, ltable, rtable);
    }

    /**
     * Checks similarity with `other`.
     * The difference with {@link #equals(const Node&)} is that we are only interested in structural similarity,
     * while we require complete equivalence with the equality check.
     * e.g. when reading a type with a value and an id, comparing 2 instances with different id but equal value,
     * then `equals()` returns `false`, as the ids are different, but `similar()` returns `true`, since structural properties are the same.
     * @param other Node to compare with.
     * @param ltable Symboltable for LHS expression ('this' node subtree).
     * @param rtable Symboltable for RHS expression (other node subtree).
     * @note If any of ltable, rtable is a `nullptr`, does not compare symbols.
     * @return `true` iff similar, `false` otherwise.
     *
     * @note When calling this function from testing code, `this` and `ltable` are the reference trees. `other` and `rtable` are student output tree and table.
     */
    inline bool similar_to(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const {
        return similar(other, ltable, rtable);
    }

    /**
     * Exactly like `similar_to(const Node&, const SymbolTable*, const SymbolTable*)`,
     * except that this function prints `this` synaxtree, up to the first found difference. The (unexpected) different value is displayed in square braces.
     * @param stream Output stream to write printed data to.
     * @param indent Indentation for current line.
     * @param indent_add Indentation to add for next indent.
     * @return `true` iff similar, `false` otherwise. Prints tree to first difference to `stream`.
     *
     * @note When calling this function from testing code, `this` and `ltable` are the reference trees. `other` and `rtable` are student output tree and table.
     */
    inline bool similar_to_debug(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable, std::ostream& stream, int indent, int indent_add) const {
        return similar_debug(other, ltable, rtable, stream, indent, indent_add);
    }

    protected:
    inline virtual bool equals(const Node& other, const SymbolTable* /*ltable*/, const SymbolTable* /*rtable*/) const {
        return nodeType == other.nodeType && returnType == other.returnType;
    }
    inline virtual bool similar(const Node& other, const SymbolTable* /*ltable*/, const SymbolTable* /*rtable*/) const {
        return nodeType == other.nodeType && returnType == other.returnType;
    }

    inline virtual bool similar_debug(const Node& other, const SymbolTable* /*ltable*/, const SymbolTable* /*rtable*/, std::ostream& /*stream*/, int /*indent*/, int /*indent_add*/) const {
        auto val = nodeType == other.nodeType && returnType == other.returnType;
        return val;
    }

    private:
    NodeType nodeType;
    ReturnType returnType;
};

class UnaryNode : public Node {
    public:
    UnaryNode() : child(nullptr) {}
    UnaryNode(NodeType nodeType, ReturnType returnType) : Node(nodeType, returnType), child(nullptr) {}

    ~UnaryNode() override { delete child; };

    Node* getChild() const;
    void setChild(Node* node);

    inline std::ostream& doStream(std::ostream& stream) const override {
        return doStream(stream, 0, 4);
    }

    inline std::ostream& doStream(std::ostream& stream, int indent, int indent_add) const override {
        return doStream(stream, indent, indent_add, nullptr);
    }

    inline std::ostream& doStream(std::ostream& stream, int indent, int indent_add, const SymbolTable* table) const override {
        Node::doStream(stream, indent, indent_add, table);
        stream << '\n';
        if (child)
            child->doStream(stream, indent + indent_add, indent_add, table);
        else
            stream << std::setw(indent + indent_add) << "(null)\n";
        return stream;
    }

    protected:
    inline bool equals(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override {
        if (const auto* unaryNode = dynamic_cast<const UnaryNode*>(&other))
            return this->Node::equals(other, ltable, rtable) && child->equal_to(*unaryNode->child, ltable, rtable);
        return false;

    }

    inline bool similar(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override {
        if (const auto* unaryNode = dynamic_cast<const UnaryNode*>(&other))
            return this->Node::similar(other, ltable, rtable) && child->similar_to(*unaryNode->child, ltable, rtable);
        return false;
    }

    inline bool similar_debug(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable, std::ostream& stream, int indent, int indent_add) const override {
        Node::doStream(stream, indent, indent_add, ltable);

        if (!this->Node::similar_debug(other, ltable, rtable, stream, indent, indent_add)) {
            stream << " [";
            other.Node::doStream(stream, 0, 0, rtable);
            stream << "]\n";
            return false;
        }
        stream << "\n";
        if (const auto* unaryNode = dynamic_cast<const UnaryNode*>(&other))
            return this->Node::similar_debug(other, ltable, rtable, stream, indent, indent_add) && child->similar_to_debug(*unaryNode->child, ltable, rtable, stream, indent+indent_add, indent_add);
       return false;
    }
private:
    Node* child;
};

class BinaryNode : public Node {
    public:
    BinaryNode() : Node(), leftChild(nullptr), rightChild(nullptr) {}
    BinaryNode(NodeType nodeType, ReturnType returnType) : Node(nodeType, returnType), leftChild(nullptr), rightChild(nullptr) {}

    ~BinaryNode() override {
        delete leftChild;
        delete rightChild;
    };

    Node* getLeftChild() const;
    void setLeftChild(Node* node);

    Node* getRightChild() const;
    void setRightChild(Node* node);

    inline std::ostream& doStream(std::ostream& stream) const override {
        return doStream(stream, 0, 4);
    }

    inline std::ostream& doStream(std::ostream& stream, int indent, int indent_add) const override {
        return doStream(stream, indent, indent_add, nullptr);
    }

    inline std::ostream& doStream(std::ostream& stream, int indent, int indent_add, const SymbolTable* table) const override {
        Node::doStream(stream, indent, indent_add, table);
        stream << '\n';
        if (leftChild)
            leftChild->doStream(stream, indent + indent_add, indent_add, table);
        else
            stream << std::setw(indent + indent_add) << "(null)\n";

        if (rightChild)
            rightChild->doStream(stream, indent + indent_add, indent_add, table);
        else
            stream << std::setw(indent + indent_add) << "(null)\n";
        return stream;
    }

    protected:
    inline bool equals(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override {
        if (const auto* binaryNode = dynamic_cast<const BinaryNode*>(&other))
            return this->Node::equals(other, ltable, rtable) && leftChild->equal_to(*binaryNode->leftChild, ltable, rtable) && rightChild->equal_to(*binaryNode->rightChild, ltable, rtable);
        return false;
    }

    inline bool similar(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override {
        if (const auto* binaryNode = dynamic_cast<const BinaryNode*>(&other))
            return this->Node::similar(other, ltable, rtable)
            && leftChild->similar_to(*binaryNode->leftChild, ltable, rtable)
            && rightChild->similar_to(*binaryNode->rightChild, ltable, rtable);
        return false;
    }

    inline bool similar_debug(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable, std::ostream& stream, int indent, int indent_add) const override {
        Node::doStream(stream, indent, indent_add, ltable);
        if (!this->Node::similar_debug(other, ltable, rtable, stream, indent, indent_add)) {
            stream << " [";
            other.Node::doStream(stream, 0, 0, rtable);
            stream << "]\n";
            return false;
        }
        stream << "\n";
        if (const auto* binaryNode = dynamic_cast<const BinaryNode*>(&other))
            return this->Node::similar_debug(other, ltable, rtable, stream, indent, indent_add)
                   && leftChild->similar_to_debug(*binaryNode->leftChild, ltable, rtable, stream, indent+indent_add, indent_add)
                   && rightChild->similar_to_debug(*binaryNode->rightChild, ltable, rtable, stream, indent+indent_add, indent_add);
        return false;
    }
private:
    Node* leftChild;
    Node* rightChild;
};

template<typename T>
class ConstantNode : public Node {
    public:
    ConstantNode() : Node() {}
    ConstantNode(NodeType nodeType, ReturnType returnType) : Node(nodeType, returnType) {}
    ConstantNode(NodeType nodeType, ReturnType returnType, T value) : Node(nodeType, returnType), value(value) {}

    T getValue() const { return value; }
    void setValue(T new_value) { value = new_value; };

    inline std::ostream& doStream(std::ostream& stream) const override {
        return doStream(stream, 0, 4);
    }

    inline std::ostream& doStream(std::ostream& stream, int indent, int indent_add) const override {
        return doStream(stream, indent, indent_add, nullptr);
    }

    inline std::ostream& doStream(std::ostream& stream, int indent, int indent_add, const SymbolTable* table) const override {
        Node::doStream(stream, indent, indent_add, table);
        return stream << "value: " << std::to_string(value) << '\n';
    }

    protected:
    inline bool equals(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override {
        if (const auto* constantNode = dynamic_cast<const ConstantNode<T>*>(&other))
            return this->Node::equals(other, ltable, rtable) && value == constantNode->value;
        return false;

    }

    inline bool similar(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override {
        if (const auto* constantNode = dynamic_cast<const ConstantNode<T>*>(&other))
            return this->Node::similar(other, ltable, rtable) && value == constantNode->value;
        return false;
    }

    inline bool similar_debug(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable, std::ostream& stream, int indent, int indent_add) const override {
        Node::doStream(stream, indent, indent_add, ltable);
        stream << " (Type: ConstantNode<" << hack::get_name<T>() << ">) value: " << std::to_string(value);
        if (!this->similar(other, ltable, rtable)) {
            stream << " [";
            if (const auto* constantNode = dynamic_cast<const ConstantNode<T>*>(&other)) {
                constantNode->doStream(stream, 0, 0, rtable);
            } else {
                if (const auto* int8Constant = dynamic_cast<const ConstantNode<int8_t>*>(&other)) {
                    int8Constant->Node::doStream(stream, 0, 0, ltable);
                    stream << " (Type: ConstantNode<int8>) value: " << std::to_string(int8Constant->getValue());
                } else if (const auto* uint8Constant = dynamic_cast<const ConstantNode<uint8_t>*>(&other)) {
                    uint8Constant->Node::doStream(stream, 0, 0, ltable);
                    stream << " (Type: ConstantNode<uint8>) value: " << std::to_string(uint8Constant->getValue());
                } else if (const auto* intConstant = dynamic_cast<const ConstantNode<int>*>(&other)) {
                    intConstant->Node::doStream(stream, 0, 0, ltable);
                    stream << " (Type: ConstantNode<int>) value: " << std::to_string(intConstant->getValue());
                } else if (const auto* uintConstant = dynamic_cast<const ConstantNode<unsigned>*>(&other)) {
                    uintConstant->Node::doStream(stream, 0, 0, rtable);
                    stream << " (Type: ConstantNode<unsigned>) value: " << std::to_string(uintConstant->getValue());
                } else {
                    other.Node::doStream(stream, 0, 0, rtable);
                }
            }
            stream << "]\n";
            return false;
        }
        stream << "\n";
        return true;
    }

private:
    T value;
};

class SymbolNode : public Node {
    public:
    SymbolNode() : Node() {}
    SymbolNode(NodeType nodeType, ReturnType returnType) : Node(nodeType, returnType) {}
    SymbolNode(NodeType nodeType, ReturnType returnType, size_t sym_id) : Node(nodeType, returnType), sym_id(sym_id) {}

    size_t getSymbolId() const;
    void setSymbolId(size_t id);

    inline std::ostream& doStream(std::ostream& stream) const override {
        return doStream(stream, 0, 4);
    }

    inline std::ostream& doStream(std::ostream& stream, int indent, int indent_add) const override {
        return doStream(stream, indent, indent_add, nullptr);
    }

    std::ostream& doStream(std::ostream& stream, int indent, int indent_add, const SymbolTable* table) const override;

    protected:
    bool equals(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override;

    bool similar(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const override;

    inline bool similar_debug(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable, std::ostream& stream, int indent, int indent_add) const override;

private:
    size_t sym_id = std::numeric_limits<size_t>::max();
};

namespace util { // Adding `to_string` implementation for IOperator, so we can use `util::to_string(ioperator)`.
    template<const Node&>
    inline std::string to_string(const Node&);
    template<const UnaryNode&>
    inline std::string to_string(const UnaryNode&);
    template<const BinaryNode&>
    inline std::string to_string(const BinaryNode&);

    template<typename T, const ConstantNode<T>&>
    inline std::string to_string(const ConstantNode<T>&);

    template<const SymbolNode&>
    inline std::string to_string(const SymbolNode&);
}
#endif
