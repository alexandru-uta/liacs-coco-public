#include "icvisitor.h"

#include <iostream>
#include <cassert>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include "ioperatortype.h"
#include "ioperand.h"
#include "ioperator.h"
#include <symbol.h>
#include <to_string.h>

ICVisitor::IOperandPtr ICVisitor::createImmediateIOperand(Node* node) {
    // TODO: implement me
    return nullptr;
}

ICVisitor::ICVisitor(SymbolTable& symtab, IntermediateCode& icode) : symtab(symtab), icode(icode), temporaries(0), labels(0) {}

void ICVisitor::emit(IOperatorType type, IOperator op, const std::shared_ptr<IOperand>& opnd1, const std::shared_ptr<IOperand>& opnd2, const std::shared_ptr<IOperand>& res) {
    icode.appendStatement(new IStatement(type, op, opnd1, opnd2, res));
}

ICVisitor::ISymbolOpPtr ICVisitor::make_temporary(ReturnType rt) {
    size_t id = symtab.addTempvar(rt, "&" + std::to_string(temporaries++), function_stack.back());
    return std::make_unique<SymbolIOperand>(id, rt);
}

ICVisitor::ISymbolOpPtr ICVisitor::make_label() {
    size_t id = symtab.addLabel(RT_VOID, "@" + std::to_string(this->labels++), function_stack.back());
    if (id == std::numeric_limits<size_t>::max())
        return nullptr;
    return std::make_unique<SymbolIOperand>(id, RT_VOID);
}

void ICVisitor::accept(Node* node) {
    // TODO: implement me
}

ICVisitor::IOperandPtr ICVisitor::accept_expr(Node* expr) {
    // TODO: implement me
    return nullptr;
}

void ICVisitor::visit_function(size_t id, Node* root) {
    // TODO: implement me
}

void ICVisitor::visit_assignment(Node* lvariable, Node* expr) {
    // TODO: implement me
}

void ICVisitor::visit_if_else(Node* cond, Node* csq, Node* alt) {
    // TODO: implement me
}

void ICVisitor::visit_while(Node* cond, Node* csq) {
    // TODO: implement me
}

ICVisitor::IOperandPtr ICVisitor::visit_func_call(BinaryNode* node) {
    // TODO: implement me
    return nullptr;
}

void ICVisitor::visit_return(Node* node) {
    // TODO: implement me
}

ICVisitor::IOperandPtr ICVisitor::visit_larray_access(BinaryNode* node) {
    // TODO: implement me
    return nullptr;
}

ICVisitor::IOperandPtr ICVisitor::visit_rarray_access(BinaryNode* node) {
    // TODO: implement me
    return nullptr;
}

void ICVisitor::visit_expr_list(Node* node) {
    // TODO: implement me
}

void ICVisitor::visit_jump_expr(Node* node, ICVisitor::IOperandPtr label) {
    // TODO: implement me
}

ICVisitor::IOperandPtr ICVisitor::visit_binary_op(BinaryNode* node) {
    // TODO: implement me
    return nullptr;
}

ICVisitor::IOperandPtr ICVisitor::visit_unary_op(UnaryNode* node) {
    // TODO: implement me
    return nullptr;
}