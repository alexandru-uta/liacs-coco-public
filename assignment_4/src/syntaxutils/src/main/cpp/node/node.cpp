#include "node.h"
#include <iostream>

NodeType Node::getNodeType() const { return nodeType; }

void Node::setNodeType(NodeType type) {
    nodeType = type;
}

ReturnType Node::getReturnType() const { return returnType; }

void Node::setReturnType(ReturnType type) {
    returnType = type;
}

Node* UnaryNode::getChild() const { return child; }

void UnaryNode::setChild(Node* childNode) {
    child = childNode;
}

Node* BinaryNode::getLeftChild() const { return leftChild; }

void BinaryNode::setLeftChild(Node* childNode) {
    leftChild = childNode;
}

Node* BinaryNode::getRightChild() const { return rightChild; }

void BinaryNode::setRightChild(Node* childNode) {
    rightChild = childNode;
}

size_t SymbolNode::getSymbolId() const { return sym_id; }

void SymbolNode::setSymbolId(size_t id) { sym_id = id; }


static std::ostream& doStreamInternal(std::ostream& stream, int indent, int indent_add, const SymbolTable* table, const SymbolNode* const node, bool newlines=false) {
    node->Node::doStream(stream, indent, indent_add, table);
    if (table) {
        const auto* sym = table->getSymbol(node->getSymbolId());
        if (sym)
            return stream << " " << sym->getName() << " (id: " << node->getSymbolId() << (newlines ? ")\n" : ")");
        return stream << " <entry not found> (id: " << node->getSymbolId() << (newlines ? ")\n" : ")");
    }
    return stream << " id: " << node->getSymbolId() << (newlines ? "\n" : "");
}

std::ostream& SymbolNode::doStream(std::ostream& stream, int indent, int indent_add, const SymbolTable* table) const {
    return doStreamInternal(stream, indent, indent_add, table, this, /*newlines=*/true);
}

bool SymbolNode::equals(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const {
    if (const auto* symbolNode = dynamic_cast<const SymbolNode*>(&other))
        return this->Node::equals(other, ltable, rtable) && (!ltable || !rtable || ltable->getSymbol(sym_id)->equal_to(*(rtable->getSymbol(symbolNode->sym_id))));
    return false;
}

bool SymbolNode::similar(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable) const {
    if (const auto* symbolNode = dynamic_cast<const SymbolNode*>(&other))
        return this->Node::similar(other, ltable, rtable) && (!ltable || !rtable || ltable->getSymbol(sym_id)->similar_to(*(rtable->getSymbol(symbolNode->sym_id))));
    return false;
}

inline bool SymbolNode::similar_debug(const Node& other, const SymbolTable* ltable, const SymbolTable* rtable, std::ostream& stream, int indent, int indent_add) const {
    doStreamInternal(stream, indent, indent_add, ltable, this, /*newlines=*/false);

    if (!this->similar(other, ltable, rtable)) {
        stream << " [";
        if (const auto* symbolNode = dynamic_cast<const SymbolNode*>(&other))
            doStreamInternal(stream, 0, 0, rtable, symbolNode, /*newlines=*/false);
        else
            other.Node::doStream(stream, 0, 0, rtable);
        stream << "]\n";
        return false;
    }
    stream << "\n";
    return true;
}

namespace util {
    template<>
    std::string to_string<const Node&>(const Node &object) {
        return "[Node] " + util::to_string(object.getReturnType()) + " " + util::to_string(object.getNodeType());
    }
    template<>
    std::string to_string<const UnaryNode&>(const UnaryNode& object) {
        return "[UnaryNode] " + util::to_string(object.getReturnType()) + " " + util::to_string(object.getNodeType());
    }
    template<>
    std::string to_string<const BinaryNode&>(const BinaryNode& object) {
        return "[BinaryNode] " + util::to_string(object.getReturnType()) + " " + util::to_string(object.getNodeType());
    }
    template<typename T, const ConstantNode<T>&>
    std::string to_string(const ConstantNode<T>& object) {
        return "[ConstantNode] " + util::to_string(object.getReturnType()) + " " + util::to_string(object.getNodeType()) + " val: " + object.getValue();
    }

    template<>
    inline std::string to_string(const SymbolNode& object) {
        return "[SymbolNode] " + util::to_string(object.getReturnType()) + " " + util::to_string(object.getNodeType()) + " id: " + std::to_string(object.getSymbolId());
    }

}