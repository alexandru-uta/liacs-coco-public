#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_IOPERATOR
#define COCO_FRAMEWORK_INTERMEDIATECODE_IOPERATOR

#include <functional>
#include <string>
#include <to_string.h>

/**
 * Operator of an IStatement
 * Several conventions are used.
 * Each IOperator has a signature, which is specified between brackets, e.g. [1-r] or [12r].
 * This signature indicates which of the operand1, operand2 and result fields have to be filled in.
 * For each (class of) IOperators, a more elaborate description about what has to be filled in is given below.
 */



typedef enum __ioperator {
    /* Meta */
    IOP_UNKNOWN, // [---] (yet) Unknown operation

    /* Subprogram entry
         operand1  should refer to the corresponding Symbol object */
    IOP_FUNC, // [1--] Subprogram

    /* Subprogram return
         operand1  should contain the value that has to be returned */
    IOP_RETURN, // [1--] Function return

    /* Parameter passing (caller side)
         operand1  should refer to the corresponding Symbol object */
    IOP_PARAM, // [1--] parameter

    /* Subprogram call
         operand1  should refer to the corresponding Symbol object
         result    receives the result of a function call */
    IOP_FUNCCALL, // [1-r] Function call

    /* Label
         operand1  should refer to the corresponding Symbol object */
    IOP_LABEL, // [1--] Label

    /* Goto
         operand1  should refer to the corresponding Symbol object; note that
                             the Symbol pointed to should be of type ST_LABEL */
    IOP_GOTO, // [1--] Goto a label

    /* Variable assignment (copy value)
         operand1  the source operand
         result    the destination operand (pointer to a Symbol object) */
    IOP_ASSIGN, // [1-r] Integer assignment

    /* Lvariable Array access
         operand1 the source operand
         operand2 index
         result symbol to array */
    IOP_LARRAY,

    /* rvariable Array access
        operand1 symbol to array
        operand2 index
        result the destination operand */
    IOP_RARRAY,

    /* Branching
         operand1  the first source operand
         operand2  the second source operand
         result    should refer to the target Symbol object; note that
                             the Symbol pointed to must be of type ST_LABEL */
    IOP_JE,               // [12r] == jump
    IOP_JNE,              // [12r] != jump
    IOP_JB,               // [12r] < unsigned jump
    IOP_JL,               // [12r] < signed jump
    IOP_JNB,              // [12r] >= unsigned jump
    IOP_JGE,              // [12r] >= signed jump
    IOP_JBE,              // [12r] <= unsigned jump
    IOP_JLE,              // [12r] <= signed jump
    IOP_JA,               // [12r] > unsigned jump
    IOP_JG,               // [12r] > signed jump
    IOP_JNZ,              // [1-r] jump not zero
    IOP_JZ,               // [1-r] jump zero

    /* Conditional Set
           operand1    the first source operand
           operand2    the second source operand
           result      the target Symbol */
    IOP_SETE,               // [12r] == set
    IOP_SETNE,              // [12r] != set
    IOP_SETG,               // [12r] > signed set
    IOP_SETGE,              // [12r] >= signed set
    IOP_SETL,               // [12r] < signed set
    IOP_SETLE,              // [12r] <= signed set
    IOP_SETA,               // [12r] > unsigned set
    IOP_SETNB,              // [12r] >= unsigned set
    IOP_SETB,               // [12r] < unsigned set
    IOP_SETBE,              // [12r] <= unsigned set

    /* Binary arithmetic operators
         operand1  the first source operand
         operand2  the second source operand
         result    the destination operand (refers to a Symbol object) */
    IOP_ADD,               // [12r] Addition
    IOP_SUB,               // [12r] Subtraction
    IOP_MUL,               // [12r] Multiplication
    IOP_DIV,               // [12r] Unsigned Division
    IOP_IDIV,              // [12r] Signed Division
    IOP_MOD,               // [12r] Modulo
    IOP_IMOD,              // [12r] Signed Modulo
    IOP_AND,               // [12r] AND operation
    IOP_OR,                // [12r] OR operation

    /* Unary arithmetic operators
         operand1  the source operand
         result    the destination operand (refers to a Symbol object) */
    IOP_NOT,         // [1-r] NOT operation
    IOP_UNARY_MINUS, // [1-r] Unary integer minus

    /* Coercion
         operand1  the source operand
         result    the destination operand (refers to a Symbol object) */
    IOP_COERCE // [1-r] Coercion
} IOperator;

namespace util { // Adding `to_string` implementation for IOperator, so we can use `util::to_string(ioperator)`.
    template<>
    std::string to_string<IOperator>(IOperator);
}

namespace std { // Adding hashfunction for IOperator to standard namespace, so we can use it in `unordered_map` etc.
    template<>
    struct hash<IOperator> {
        size_t operator()(IOperator op) const {
            return static_cast<size_t>(op);
        }
    };
}

#endif
