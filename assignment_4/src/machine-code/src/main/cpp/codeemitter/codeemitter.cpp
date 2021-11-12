#include "codeemitter.h"
#include <ioperand.h>
#include <iostream>
#include <stdexcept>
#include <types.h>
#include <utility.h>

void CodeEmitter::clean(unsigned line_after_clean) {
    this->line = line_after_clean;
    //TODO: implement me
}

void CodeEmitter::emit_prologue() {
    //TODO: implement me
}

void CodeEmitter::emit_epilogue() {
    //TODO: implement me
}

void CodeEmitter::emit_store(size_t dst, const std::shared_ptr<IOperand>& src) {
    //TODO: implement me
}

void CodeEmitter::emit_return(const std::shared_ptr<IOperand>& src) {
    //TODO: implement me
}

void CodeEmitter::emit_uminus(size_t dst, const std::shared_ptr<IOperand> &src) {
    //TODO: implement me
}

void CodeEmitter::emit_label(size_t sym) {
    //TODO: implement me
}

void CodeEmitter::emit_binop(IStatement* stmt) {
    //TODO: implement me
}

void CodeEmitter::emit_div(IOperator op, const std::shared_ptr<IOperand>& lhs, const std::shared_ptr<IOperand>& rhs, size_t dst_div, size_t dst_mod) {
    //TODO: implement me
}

void CodeEmitter::emit_not(size_t dst, const std::shared_ptr<IOperand>& src) {
    //TODO: implement me
}

void CodeEmitter::emit_cast(size_t dst, const std::shared_ptr<IOperand>& src) {
    //TODO: implement me
}

void CodeEmitter::emit_param(const std::shared_ptr<IOperand>& opr) {
    //TODO: implement me
}

void CodeEmitter::emit_funccall(size_t func, size_t dst) {
    //TODO: implement me
}

void CodeEmitter::emit_func(size_t func) {
    //TODO: implement me
}

void CodeEmitter::emit_set(IOperator op, const std::shared_ptr<IOperand>& op1, const std::shared_ptr<IOperand>& op2, const std::shared_ptr<SymbolIOperand>& result) {
    //TODO: implement me
}

void CodeEmitter::emit_relop(IOperator op, const std::shared_ptr<IOperand>& op1, const std::shared_ptr<IOperand>& op2, const std::shared_ptr<SymbolIOperand>& label) {
    //TODO: implement me
}

void CodeEmitter::emit_jump_cond(IOperator op, const std::shared_ptr<IOperand> &op1, const std::shared_ptr<SymbolIOperand> &label) {
    //TODO: implement me
}

void CodeEmitter::emit_jump(size_t sym) {
    //TODO: implement me
}

void CodeEmitter::emit_rarray_access(const std::shared_ptr<SymbolIOperand> &array_sym, const std::shared_ptr<IOperand> &index, const std::shared_ptr<SymbolIOperand> &dest) {
    //TODO: implement me
}

void CodeEmitter::emit_larray_access(const std::shared_ptr<SymbolIOperand>& src, const std::shared_ptr<IOperand>& index, const std::shared_ptr<SymbolIOperand>& array_sym) {
    //TODO: implement me
}













