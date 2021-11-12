#include <iostream>
#include <to_string.h>
#include "syntaxtree.h"

SyntaxTree::~SyntaxTree() {
    for (auto& func: functions) {
        delete func.second.root;
    }
}

Node* SyntaxTree::getRoot(size_t id) const {
    auto it = functions.find(id);
    if (it == functions.end())
        return nullptr;
    return it->second.root;
}

Node* SyntaxTree::createParentNode(NodeType nodeType, ReturnType returnType, Node* child) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxTree::createParentNode(NodeType nodeType, ReturnType ret, Node* leftChild, Node* rightChild) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxTree::createIdNode(size_t id, ReturnType rt) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxTree::createLeaf() {
    Node* node = new Node;

    node->setNodeType(NODE_EMPTY);
    node->setReturnType(RT_VOID);

    return node;
}
