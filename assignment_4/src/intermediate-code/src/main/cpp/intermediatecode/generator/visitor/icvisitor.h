#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_ICVISITOR
#define COCO_FRAMEWORK_INTERMEDIATECODE_ICVISITOR

#include "intermediatecode.h"
#include "ioperand.h"
#include <cstddef>
#include <memory>
#include <node.h>
#include <symboltable.h>
#include <types.h>

class ICVisitor {
    protected:
    // Utility alias for returning an IOperand, to ease memory management.
    using IOperandPtr = std::unique_ptr<IOperand>;
    using ISymbolOpPtr = std::unique_ptr<SymbolIOperand>;

    SymbolTable& symtab;
    IntermediateCode& icode;
    size_t temporaries, labels;
    std::vector<size_t> function_stack;

    //helper function
    static IOperandPtr createImmediateIOperand(Node* node);

    public:
    ICVisitor(SymbolTable& symtab, IntermediateCode& icode);

    // Utility function to emit a new instruction to the intermediate code.
    void emit(IOperatorType type, IOperator op, const std::shared_ptr<IOperand>& opnd1, const std::shared_ptr<IOperand>& opnd2, const std::shared_ptr<IOperand>& res);

    // Utility function to create a new unique temporary variable, of type rt
    ISymbolOpPtr make_temporary(ReturnType rt);

    // Utility function to create a unique label.
    ISymbolOpPtr make_label();

    // Process and generate intermediate code for a node tree.
    void accept(Node* node);

    // Process and generate intermediate code for an expression subtree.
    IOperandPtr accept_expr(Node* expr);

    // Process and generate intermediate code for a function.
    void visit_function(size_t id, Node* root);

    // Generate code for an assignment node
    void visit_assignment(Node* lvariable, Node* expr);

    // Generate code for an if-statement.
    void visit_if_else(Node* cond, Node* csq, Node* alt);

    // Generate code for a while-statement.
    void visit_while(Node* cond, Node* csq);

    // Generate code for a function call node.
    IOperandPtr visit_func_call(BinaryNode* node);

    // Generate code for a function return.
    // Expects the child of the NODE_RETURN
    void visit_return(Node* node);

    /**
     * Generate code for array access
     * @param node node to the NODE_LARRAY
     * @return the generated IOperandPtr
     */
    IOperandPtr visit_larray_access(BinaryNode* node);

    /**
     * Generate code for array access
     * @param node node to the NODE_RARRAY
     * @return the generated IOperandPtr
     */
    IOperandPtr visit_rarray_access(BinaryNode* node);

    // Generate code for an expression list. Each expression is emitted as a parameter.
    void visit_expr_list(Node* node);

    // Generate code for a Jump expression
    void visit_jump_expr(Node* node, IOperandPtr label);

    // Generate code for a binary operator.
    IOperandPtr visit_binary_op(BinaryNode* node);

    // Generate code for a unary operator.
    IOperandPtr visit_unary_op(UnaryNode* node);
};

#endif
