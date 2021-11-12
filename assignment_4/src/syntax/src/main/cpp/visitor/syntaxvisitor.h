#ifndef COCO_FRAMEWORK_SYNTAX_SYNTAXVISITOR
#define COCO_FRAMEWORK_SYNTAX_SYNTAXVISITOR

#include <sstream>
#include <stack>
#include <string>

#include <basevisitor.h>
#include <decllist.h>
#include <frontendbuilder.h>
#include <listtree.h>

#include "logger.h"
#include "node.h"

/** A visitor class to handle tree construction. Be aware that, in case of errors, an error node is returned and all parent nodes constructed further on will be error nodes. */
class SyntaxVisitor : public BaseVisitor {
    public:
    SyntaxVisitor(Logger& logger, SymbolTable& table, SyntaxTree& tree) : BaseVisitor(logger), builder(table, tree){};

    /**
     * Function to add builtin C-Minus functions (such as 'readinteger')
     */
    void add_builtins() override;

    void program_start() override;

    /**
     * Visitor function to visit main program
     */
    void visit_program() override;

    /**
     * Visitor function for function start
     * @param rt Returntype of the function
     * @param name Name of the function
     */
    void visit_func_start(ReturnType rt, const std::string& name) override;

    /**
     * Visitor function for function end
     * @param name Name of the function
     * @param root Node* to the body of the function
     */
    void visit_func_end(const std::string& name, Node* root) override;

    /**
     * Visitor function for function calls
     * @param name Name of the function
     * @param exprlist Node* to the arguments of the functioncall
     * @return created NODE_FUNCTIONCALL node
     */
    Node* visit_funccall(const std::string& name, Node* exprlist) override;

    /**
     * Visitor function for statement lists
     * @param stmt Node* to current statement
     * @param stmtlist Node* to the rest of the statements
     * @return created NODE_STATEMENT_LIST node
     */
    void visit_statement_list(Node* stmt, ListTree& stmtlist) const override;

    /**
     * Visitor function to handle assignments
     * @param var Node* to the LHS of the assignment
     * @param expr Node* to the RHS of the assignment
     * @return created NODE_ASSIGNMENT node
     */
    Node* visit_assignment(Node* var, Node* expr) override;

    /**
     * Visitor function for lvariables
     * @param name Name of the variable
     * @return created NODE_ID node
     */
    Node* visit_lvariable(const std::string& name) override;

    /**
       * Visitor function for lvariables
       * @param id Id of the variable
       * @return created NODE_ID node
       */
    Node* visit_lvariable(size_t id) override;

    /**
     * Registers the gathered declarations
     * @param rt Common ReturnType of all gathered declarations
     */
    void register_declarations(ReturnType rt) override;

    /**
     * Visitor function for variable declaration (where ReturnType is not yet known)
     * @param name Name of the declared variable
     */
    void visit_var_decl(const std::string& name) override;

    /**
     * Visitor function for array variable declaration (where ReturnType is not yet known)
     * @param name Name of the declared array
     * @param size_string Array size
     */
    void visit_array_decl(const std::string& name, const std::string& size_string) override;
    /**
     * Visitor function for lvariable arrays
     * @param name Name of the array
     * @param index Index of access
     * @return created NODE_ARRAY
     */
    Node* visit_lvariable(const std::string& name, Node* index) override;

    /**
     * Visitor function for rvariables
     * @param name Name of the variable
     * @return created NODE_ID node
     */
    Node* visit_rvariable(const std::string& name) override;

    /**
     * Visitor function for rvariable arrays
     * @param name Name of the array
     * @param index Index of access
     * @return created NODE_ARRAY
     */
    Node* visit_rvariable(const std::string& name, Node* index) override;

    /**
     * Visitor function for variable/ parameter declarations
     * @param name Name of the declared variable/ parameter
     * @param st SymbolType of the declared variable/ parameter
     * @param rt ReturnType of the declared variable/ parameter
     * @return id of the new created symbol
     */
    size_t visit_decl(const std::string& name, SymbolType st, ReturnType rt) override;

    /**
     * Visitor function for array declarations
     * @param name Name of the array
     * @param st SymbolType either ST_VARIABLE or ST_PARAMETER
     * @param rt ReturnType of the array
     * @param size Array size
     * @return id of the new created symbol
     */
    size_t visit_array_decl(const std::string& name, SymbolType st, ReturnType rt, const std::string& size_string) override;

    /**
     * Visitor function to interpret strings to numbers (if possible)
     * @param number String of the number to convert
     * @return created NODE_NUM_INT node
     */
    Node* visit_number(const std::string& number) override;

    /**
     * Visitor function to handle if statements
     * @param boolexpr Node* to condition to evaluate
     * @param stmt Node* to the body of the statement
     * @param opt_else_stmt Node* to the optional body of the else
     * @return created NODE_IF node
     */
    Node* visit_if(Node* boolexpr, Node* stmt, Node* opt_else_stmt) override;

    /**
     * Visitor function to handle while statements
     * @param boolexpr Node* to condition to evaluate
     * @param stmt Node* to the body of the statement
     * @return created NODE_WHILE node
     */
    Node* visit_while(Node* boolexpr, Node* stmt) override;

    /**
     * Visitor function to handle return statements
     * @param expr Node* to the expression to return
     * @return created NODE_RETURN node
     */
    Node* visit_return(Node* expr) override;

    /**
     * Visitor function to handle expression lists
     * @param expr Node* to the current expression
     * @param exprlist contains the root of the expression list, and the current position
     */
    void visit_exprlist(Node* expr, ListTree& exprlist) const override;

    /**
     * Visitor function for binary operators. Where needed, converts `lhs` and `rhs` nodes
     * @param op NodeType of the operator
     * @param lhs Node* to the LHS of the operator
     * @param rhs Node* to the RHS of the operator
     * @return Node* to the newly created node
     */
    Node* visit_operator(NodeType op, Node* lhs, Node* rhs) override;

    /**
     * Visitor function for unary operators. Where needed, converts `arg` nodes
     * @param op NodeType of the operator
     * @param arg Node* to the argument
     * @return Node* to the newly created node
     */
    Node* visit_operator(NodeType op, Node* arg) override;

    /**
     * Visitor function to build an empty node
     * @return created NODE_EMPTY node
     */
    Node* empty() const override;

    /**
     * Visitor function to add a new scope to our current scope
     */
    void add_local_scope() override;

    /**
     * Visitor function to leave the current scope
     * Does nothing if current scope is the global scope
     */
    void leave_current_scope() override;

    /**
     * Use as a debugger function i.c.w. gdb
     * @param node Node* to debug
     * @return the Node
     */
    Node* do_nothing(Node* node) override;

    private:
    //TODO: Add functions and variables to help you

    // A builder for the symboltable
    FrontendBuilder builder;

    //Declaration list for defining multiple variables in one line
    DeclList declList;
};

#endif
