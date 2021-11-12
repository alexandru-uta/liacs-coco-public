#ifndef COCO_FRAMEWORK_MACHINECODE_CODEEMITTER
#define COCO_FRAMEWORK_MACHINECODE_CODEEMITTER

#include "../allocator/globals/globalsallocator.h"
#include <ioperand.h>
#include <memory>
#include <ostream>
#include <symbol.h>
#include <unordered_map>
#include <utility.h>
#include <vector>

// Class for generating x86-64 assembly.
class CodeEmitter {
    // The output to which to print the assembly.
    std::ostream& out;

    // Information about the layouts of the globals in the data section.
    const GlobalsAllocator& globals;

    // Symbol Table
    SymbolTable& tab;

    // The current line in the intermediary code.
    unsigned line;

    public:
    // Create a CodeEmitter.
    CodeEmitter(std::ostream& out, const GlobalsAllocator& globals, SymbolTable& tab) : out(out), globals(globals), tab(tab), line(0) {}

    // Clear all allocated variables that are out of scope (not alive anymore according to the lifetime analyzer).
    void clean(unsigned line_after_clean);

    public:
    /**
     * Emit the prologue of a function
     */
    void emit_prologue();

    /**
     * Emit the epilogue of a function
     */
    void emit_epilogue();

    /**
     * Emits instructions to store the value of `src` into symbol `dst`
     * @param dst the symbol to which the value should be stored
     * @param src the operand which contains the value to store
     */
    void emit_store(size_t dst, const std::shared_ptr<IOperand>& src);

    /**
     * Emit instruction to make a function return to its caller
     * @param src the operand containing the value to return, or nullptr if no value needs to be returned
     */
    void emit_return(const std::shared_ptr<IOperand>& src);

    /**
     * Emits unary minus instruction
     * @param dst the symbol to which the operation result should be stored
     * @param src the operand to which the operation should be applied
     */
    void emit_uminus(size_t dst, const std::shared_ptr<IOperand>& src);

    /**
     * Emits logical not instruction
     * @param dst the symbol to which the operation result should be stored
     * @param src the operand to which the operation should be applied
     */
    void emit_not(size_t dst, const std::shared_ptr<IOperand>& src);

    /**
     * Emits div instruction
     * Note: Expects at least one of dst_div or dst_mod to be provided
     * @param op the operator, either DIV or IDIV
     * @param lhs the left hand side to which the operation should be applied
     * @param rhs the right hand side to which the operation should be applied
     * @param dst_div (optional) the symbol to which the divide operation result should be stored
     * @param dst_mod (optional) the symbol to which the modulo operation result should be stored
     */
    void emit_div(IOperator op, const std::shared_ptr<IOperand>& lhs, const std::shared_ptr<IOperand>& rhs, size_t dst_div = std::numeric_limits<size_t>::max(), size_t dst_mod = std::numeric_limits<size_t>::max());

    /**
     * Emits cast instructions to cast the `src` to the `dst` type
     * @param dst the symbol to which the operation result should be stored
     * @param src the operand to which the operation should be applied
     */
    void emit_cast(size_t dst, const std::shared_ptr<IOperand>& src);

    /**
     * Emits a label
     * @param sym the label identifier to emit
     */
    void emit_label(size_t sym);

    /**
     * Emits the binary operation specified by the provided IStatement
     * NOTE: use emit_div() for the IOP_MOD, IOP_IMOD, IOP_DIV and IOP_IDIV operation
     * @param stmt the statement containing the information for the binary operator
     */
    void emit_binop(IStatement* stmt);

    /**
     * Handles a parameter instruction
     * @param opr the operand containing the value for the parameter
     */
    void emit_param(const std::shared_ptr<IOperand>& opr);

    /**
     * Emit function call instructions
     * @param func the identifier of the function being called
     * @param dst the identifier of the symbol to which the function-result should be stored,
     * or nullptr if there is no function-result
     */
    void emit_funccall(size_t func, size_t dst);

    /**
     * Emits the start of a function
     * @param func the identifier of the function
     */
    void emit_func(size_t func);

    /**
     * Emits a SET operation
     * @param op the IOperator to emit
     * @param op1 the left hand side to which the operation should be applied
     * @param op2 the right hand side to which the operation should be applied
     * @param result the SymbolIOperand which saves the result of the operation
     */
    void emit_set(IOperator op, const std::shared_ptr<IOperand>& op1, const std::shared_ptr<IOperand>& op2, const std::shared_ptr<SymbolIOperand>& result);

    /**
     * Emits a relational operator instruction
     * @param op the IOperator to emit
     * @param op1 the left hand side to which the operation should be applied
     * @param op2 the right hand side to which the operation should be applied
     * @param label the label to jump to if the condition holds
     */
    void emit_relop(IOperator op, const std::shared_ptr<IOperand>& op1, const std::shared_ptr<IOperand>& op2, const std::shared_ptr<SymbolIOperand>& label);

    /**
     * Emits a conditional jump instruction
     * @param op the IOperator to emit
     * @param op1 the operand to test
     * @param label the label to jump to if the condition holds
     */
    void emit_jump_cond(IOperator op, const std::shared_ptr<IOperand>& op1, const std::shared_ptr<SymbolIOperand>& label);

    /**
     * Emits an unconditional jump instruction
     * @param sym the identifier of the label to jump to
     */
    void emit_jump(size_t sym);

    /**
     * Emits a lvariable array access
     * @param array_sym the SymbolIOperand containing the identifier of the array
     * @param index the IOperand containing the index to access
     * @param dest the SymbolIOperand containing the symbol to which the array value should be saved
     */
    void emit_rarray_access(const std::shared_ptr<SymbolIOperand>& array_sym, const std::shared_ptr<IOperand>& index, const std::shared_ptr<SymbolIOperand>& dest);

    /**
     * Emits a rvariable array access
     * @param src the SymbolIOperand containing the value which should be saved in the array
     * @param index the IOperand containing the index to which to save the value
     * @param array_sym the SymbolIOperand containing the identifier of the array
     */
    void emit_larray_access(const std::shared_ptr<SymbolIOperand>& src, const std::shared_ptr<IOperand>& index, const std::shared_ptr<SymbolIOperand>& array_sym);
};

#endif
