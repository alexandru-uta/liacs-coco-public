#include "nothingvisitor.h"

void NothingVisitor::add_builtins() {
}
void NothingVisitor::program_start() {
}
void NothingVisitor::visit_program() {
}
void NothingVisitor::visit_func_start(ReturnType, const std::string&) {
}
void NothingVisitor::visit_func_end(const std::string&, Node*) {
}
Node* NothingVisitor::visit_funccall(const std::string&, Node*) {
    return nullptr;
}
void NothingVisitor::visit_statement_list(Node*, ListTree&) const {
}
Node* NothingVisitor::visit_assignment(Node*, Node*) {
    return nullptr;
}
Node* NothingVisitor::visit_lvariable(const std::string&) {
    return nullptr;
}
Node* NothingVisitor::visit_lvariable(size_t) {
    return nullptr;
}
Node* NothingVisitor::visit_lvariable(const std::string&, Node*) {
    return nullptr;
}
Node* NothingVisitor::visit_rvariable(const std::string&) {
    return nullptr;
}
Node* NothingVisitor::visit_rvariable(const std::string&, Node*) {
    return nullptr;
}
void NothingVisitor::register_declarations(ReturnType) {
}
void NothingVisitor::visit_var_decl(const std::string&) {
}
void NothingVisitor::visit_array_decl(const std::string&, const std::string&) {
}
size_t NothingVisitor::visit_decl(const std::string&, SymbolType, ReturnType) {
    return 0;
}
size_t NothingVisitor::visit_array_decl(const std::string&, SymbolType, ReturnType, const std::string&) {
    return 0;
}
Node* NothingVisitor::visit_number(const std::string&) {
    return nullptr;
}
Node* NothingVisitor::visit_if(Node*, Node*, Node*) {
    return nullptr;
}
Node* NothingVisitor::visit_while(Node*, Node*) {
    return nullptr;
}
Node* NothingVisitor::visit_return(Node*) {
    return nullptr;
}
void NothingVisitor::visit_exprlist(Node*, ListTree&) const {
}
Node* NothingVisitor::visit_operator(NodeType, Node*, Node*) {
    return nullptr;
}
Node* NothingVisitor::visit_operator(NodeType, Node*) {
    return nullptr;
}
void NothingVisitor::add_local_scope() {
}
void NothingVisitor::leave_current_scope() {
}
Node* NothingVisitor::empty() const {
    return nullptr;
}
