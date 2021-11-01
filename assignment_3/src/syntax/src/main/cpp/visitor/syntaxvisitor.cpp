#include <vector>
#include <string>
#include <algorithm>

#include "logger.h"
#include "symboltable.h"
#include "syntaxtree.h"
#include "syntaxvisitor.h"
#include "types.h"

#include <to_string.h>

extern int lineno;

void SyntaxVisitor::add_builtins() {
    auto* sym = new Symbol();
    sym->setName("readinteger");
    sym->setSymbolType(ST_FUNCTION);
    sym->setReturnType(RT_INT);
    sym->setLine(-1);
    builder.enterFunction(sym);
    builder.leaveFunction();

    sym = new Symbol();
    sym->setName("writeinteger");
    sym->setSymbolType(ST_FUNCTION);
    sym->setReturnType(RT_VOID);
    sym->setLine(-1);
    builder.enterFunction(sym);
    visit_decl("i", ST_PARAMETER, RT_INT);
    builder.leaveFunction();

    sym = new Symbol();
    sym->setName("readunsigned");
    sym->setSymbolType(ST_FUNCTION);
    sym->setReturnType(RT_UINT);
    sym->setLine(-1);
    builder.enterFunction(sym);
    builder.leaveFunction();

    sym = new Symbol();
    sym->setName("writeunsigned");
    sym->setSymbolType(ST_FUNCTION);
    sym->setReturnType(RT_VOID);
    sym->setLine(-1);
    builder.enterFunction(sym);
    visit_decl("i", ST_PARAMETER, RT_UINT);
    builder.leaveFunction();
}

void SyntaxVisitor::program_start() {
    builder.enterNewScope();
    add_builtins();
}

void SyntaxVisitor::visit_program() {
    builder.exitCurrentScope();
}

void SyntaxVisitor::visit_func_start(ReturnType rt, const std::string& name) {
    //TODO: implement me
}

void SyntaxVisitor::visit_func_end(const std::string& name, Node* root) {
    //TODO: implement me
}

Node* SyntaxVisitor::visit_funccall(const std::string& name, Node* exprlist) {
    //TODO: implement me
    return nullptr;
}

void SyntaxVisitor::visit_statement_list(Node* stmt, ListTree& stmtlist) const {
    //TODO: implement me
}

Node* SyntaxVisitor::visit_assignment(Node* var, Node* expr) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_lvariable(size_t id) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_lvariable(const std::string& name) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_lvariable(const std::string& name, Node* index) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_rvariable(const std::string& name) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_rvariable(const std::string& name, Node* index) {
    //TODO: implement me
    return nullptr;
}

size_t SyntaxVisitor::visit_decl(const std::string& name, SymbolType st, ReturnType rt) {
    //TODO: implement me
    return std::numeric_limits<size_t>::max();
}

size_t SyntaxVisitor::visit_array_decl(const std::string& name, SymbolType st, ReturnType rt, const std::string& size_string) {
    //TODO: implement me
    return std::numeric_limits<size_t>::max();
}

Node* SyntaxVisitor::visit_number(const std::string& number) {
    // TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_if(Node* boolexpr, Node* stmt, Node* opt_else_stmt) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_while(Node* boolexpr, Node* stmt) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_return(Node* expr) {
    //TODO: implement me
    return nullptr;
}

void SyntaxVisitor::visit_exprlist(Node* expr, ListTree& exprlist) const {
    //TODO: implement me
}

Node* SyntaxVisitor::visit_operator(NodeType op, Node* lhs, Node* rhs) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::visit_operator(NodeType op, Node* arg) {
    //TODO: implement me
    return nullptr;
}

Node* SyntaxVisitor::empty() const {
    //TODO: implement me
    return nullptr;
}

void SyntaxVisitor::add_local_scope() {
    //TODO: implement me
}

void SyntaxVisitor::leave_current_scope() {
    //TODO: implement me
}


Node* SyntaxVisitor::do_nothing(Node* node) {
    return node;
}

void SyntaxVisitor::register_declarations(ReturnType rt) {
    //TODO: implement me
}

void SyntaxVisitor::visit_var_decl(const std::string& name) {
    //TODO: implement me
}

void SyntaxVisitor::visit_array_decl(const std::string& name, const std::string& size_string) {
    //TODO: implement me
}



