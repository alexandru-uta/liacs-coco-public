#ifndef COCO_FRAMEWORK_LEXICAL_BASEVISITOR
#define COCO_FRAMEWORK_LEXICAL_BASEVISITOR

#include <string>

#include <listtree.h>
#include <logger.h>
#include <node.h>
#include <types.h>

class BaseVisitor {
    public:
    explicit BaseVisitor(Logger& logger) : logger(logger){};
    virtual ~BaseVisitor() = default;

    /**
     * Function to add builtin Pascal functions (such as 'readinteger')
     */
    virtual void add_builtins() = 0;

    /**
     * Visitor function for program start
     */
    virtual void program_start() = 0;

    /**
     * Visitor function to visit main program
     * @param name Name of the main program
     * @param body Node* to the body of the main program
     */
    virtual void visit_program() = 0;

    /**
     * Visitor function for function start
     * @param rt Returntype of the function
     * @param name Name of the function
     */
    virtual void visit_func_start(ReturnType rt, const std::string& name) = 0;

    /**
     * Visitor function for function end
     * @param name Name of the function
     * @param root Node* to the body of the function
     */
    virtual void visit_func_end(const std::string& name, Node* root) = 0;

    /**
     * Visitor function for function calls
     * @param name Name of the function
     * @param exprlist Node* to the arguments of the functioncall
     * @return created NODE_FUNCTIONCALL node
     */
    virtual Node* visit_funccall(const std::string& name, Node* exprlist) = 0;

    /**
     * Visitor function for statement lists
     * @param stmt Node* to current statement
     * @param stmtlist Node* to the rest of the statements
     * @return created NODE_STATEMENT_LIST node
     */
    virtual void visit_statement_list(Node* stmt, ListTree& stmtlist) const = 0;
    /**
     * Visitor function to handle assignments
     * @param var Node* to the LHS of the assignment
     * @param expr Node* to the RHS of the assignment
     * @return created NODE_ASSIGNMENT node
     */
    virtual Node* visit_assignment(Node* var, Node* expr) = 0;

    /**
     * Visitor function for lvariables
     * @param name Name of the variable
     * @return created NODE_ID node
     */
    virtual Node* visit_lvariable(const std::string& name) = 0;

    /**
     * Visitor function for lvariables
     * @param id Id of the variable
     * @return created NODE_ID node
     */
    virtual Node* visit_lvariable(size_t id) = 0;

    /**
     * Visitor function for lvariable arrays
     * @param name Name of the array
     * @param index Index of access
     * @return created NODE_ARRAY
     */
    virtual Node* visit_lvariable(const std::string& name, Node* index) = 0;

    /**
     * Visitor function for rvariables
     * @param name Name of the variable
     * @return created NODE_ID node
     */
    virtual Node* visit_rvariable(const std::string& name) = 0;

    /**
     * Visitor function for rvariable arrays
     * @param name Name of the array
     * @param index Index of access
     * @return created NODE_ARRAY
     */
    virtual Node* visit_rvariable(const std::string& name, Node* index) = 0;

    /**
     * Registers the gathered declarations
     * @param rt Common ReturnType of all gathered declarations
     */
    virtual void register_declarations(ReturnType rt) = 0;

    /**
     * Visitor function for variable declaration (where ReturnType is not yet known)
     * @param name Name of the declared variable
     */
    virtual void visit_var_decl(const std::string& name) = 0;

    /**
     * Visitor function for array variable declaration (where ReturnType is not yet known)
     * @param name Name of the declared array
     * @param size_string Array size
     */
    virtual void visit_array_decl(const std::string& name, const std::string& size_string) = 0;

    /**
     * Visitor function for parameter declarations
     * @param name Name of the declared parameter
     * @param st ST_VARIABLE or ST_PARAMETER
     * @param rt ReturnType of the declared variable/ parameter
     * @return id of the new created symbol
     */
    virtual size_t visit_decl(const std::string& name, SymbolType st, ReturnType rt) = 0;

    /**
     * Visitor function for array parameter declarations
     * @param name Name of the array
     * @param st ST_VARIABLE or ST_PARAMETER
     * @param rt ReturnType of the array
     * @param size Array size
     * @return id of the new created symbol
     */
    virtual size_t visit_array_decl(const std::string& name, SymbolType st, ReturnType rt, const std::string& size_string) = 0;

    /**
     * Visitor function to interpret strings to numbers (if possible)
     * @param number String of the number to convert
     * @return created NODE_NUM_INT node
     */
    virtual Node* visit_number(const std::string& number) = 0;

    /**
     * Visitor function to handle if statements
     * @param boolexpr Node* to condition to evaluate
     * @param stmt Node* to the body of the statement
     * @param opt_else_stmt Node* to the optional body of the else
     * @return created NODE_IF node
     */
    virtual Node* visit_if(Node* boolexpr, Node* stmt, Node* opt_else_stmt) = 0;

    /**
     * Visitor function to handle while statements
     * @param boolexpr Node* to condition to evaluate
     * @param stmt Node* to the body of the statement
     * @return created NODE_WHILE node
     */
    virtual Node* visit_while(Node* boolexpr, Node* stmt) = 0;

    /**
     * Visitor function to handle return statements
     * @param expr Node* to the expression to return
     * @return created NODE_RETURN node
     */
    virtual Node* visit_return(Node* expr) = 0;

    /**
     * Visitor function to handle expression lists
     * @param expr Node* to the current expression
     * @param exprlist contains the root of the expression list, and the current position
     */
    virtual void visit_exprlist(Node* expr, ListTree& exprlist) const = 0;

    /**
     * Visitor function for binary operators. Where needed, converts `lhs` and `rhs` nodes
     * @param op NodeType of the operator
     * @param lhs Node* to the LHS of the operator
     * @param rhs Node* to the RHS of the operator
     * @return Node* to the newly created node
     */
    virtual Node* visit_operator(NodeType op, Node* lhs, Node* rhs) = 0;

    /**
     * Visitor function for unary operators. Where needed, converts `arg` nodes
     * @param op NodeType of the operator
     * @param arg Node* to the argument
     * @return Node* to the newly created node
     */
    virtual Node* visit_operator(NodeType op, Node* arg) = 0;

    /**
     * Visitor function to build an empty node
     * @return created NODE_EMPTY node
     */
    virtual Node* empty() const = 0;

    /**
     * Visitor function to add a new scope to our current scope
     */
    virtual void add_local_scope() = 0;

    /**
     * Visitor function to leave the current scope
     * Does nothing if current scope is the global scope
     */
    virtual void leave_current_scope() = 0;

    /**
     * Use as a debugger function i.c.w. e.g. gdb
     * @param node Node* to debug
     * @return the Node
     */
    virtual Node* do_nothing(Node* node);

    //A reference to the logger. Required to print errors, warnings and info as needed
    Logger& logger;
};

#endif
