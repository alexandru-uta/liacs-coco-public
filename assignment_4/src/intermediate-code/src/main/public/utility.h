#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_UTILITY
#define COCO_FRAMEWORK_INTERMEDIATECODE_UTILITY

#include "ioperator.h"
#include "istatement.h"
#include <ostream>
#include <utility>

// Return the number of operands that may be a value of an operator.
// That means GOTO, LABEL, FUNCCALL have 0 operands.
int iop_arity(IOperator op);

// Return whether the operator returns a value
bool iop_has_result(IOperator op);

// Return whether the operator is a goto/ jump
bool iop_is_goto(IOperator op);

// Return whether the operator is a conditional jump
bool iop_is_cond_jmp(IOperator op);

// Return whether the statement reads a variable at operand1
bool iop_reads_op1(const SymbolTable& tab, IStatement* stmt);

// Return whether the statement reads a variable at operand 2
bool iop_reads_op2(const SymbolTable& tab, IStatement* stmt);

// Return whether the statement writes a variable to result
bool iop_writes_result(const SymbolTable& tab, IStatement* stmt);

// Utility function to print a container, nicely formatted.
// Function `f` (signature: void(std::ostream&, T::value_type)) is called when a container item needs to be printed.
template<typename T, typename F>
std::ostream& print_container(std::ostream& os, T& container, F f) {
    os << '[';
    bool first = true;

    for (auto& elem: container) {
        if (first)
            first = false;
        else
            os << ", ";
        f(os, elem);
    }

    return os << ']';
}

// Format a function label. This is to avoid conflicts if a function
// is defined that is for example called "main".
inline std::string fmt_func(const std::string& fn) {
    return 'F' + fn;
}

#endif
