#ifndef COCO_FRAMEWORK_LEXICAL_NOTHINGVISITOR
#define COCO_FRAMEWORK_LEXICAL_NOTHINGVISITOR

#include "basevisitor.h"
#include <logger.h>

/** A visitor that does absolutely nothing. */
class NothingVisitor : public BaseVisitor {
    public:
    explicit NothingVisitor(Logger& logger) : BaseVisitor(logger) {}
    void add_builtins() override;
    void program_start() override;
    void visit_program() override;
    void visit_func_start(ReturnType rt, const std::string& name) override;
    void visit_func_end(const std::string& name, Node* root) override;
    Node* visit_funccall(const std::string& name, Node* exprlist) override;
    void visit_statement_list(Node* stmt, ListTree& stmtlist) const override;
    Node* visit_assignment(Node* var, Node* expr) override;
    Node* visit_lvariable(const std::string& name) override;
    Node* visit_lvariable(size_t id) override;
    Node* visit_lvariable(const std::string& name, Node* index) override;
    Node* visit_rvariable(const std::string& name) override;
    Node* visit_rvariable(const std::string& name, Node* index) override;
    void register_declarations(ReturnType rt) override;
    void visit_var_decl(const std::string& name) override;
    void visit_array_decl(const std::string& name, const std::string& size_string) override;
    size_t visit_decl(const std::string& name, SymbolType st, ReturnType rt) override;
    size_t visit_array_decl(const std::string& name, SymbolType st, ReturnType rt, const std::string& size_string) override;
    Node* visit_number(const std::string& number) override;
    Node* visit_if(Node* boolexpr, Node* stmt, Node* opt_else_stmt) override;
    Node* visit_while(Node* boolexpr, Node* stmt) override;
    Node* visit_return(Node* expr) override;
    void visit_exprlist(Node* expr, ListTree& exprlist) const override;
    Node* visit_operator(NodeType op, Node* lhs, Node* rhs) override;
    Node* visit_operator(NodeType op, Node* arg) override;
    Node* empty() const override;
    void add_local_scope() override;
    void leave_current_scope() override;
};

#endif
