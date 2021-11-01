#include "utility.h"

int iop_arity(IOperator op) {
    switch (op) {
    case IOP_ADD:
    case IOP_SUB:
    case IOP_MUL:
    case IOP_DIV:
    case IOP_MOD:
    case IOP_AND:
    case IOP_OR:
    case IOP_JE:
    case IOP_JNE:
    case IOP_JB:
    case IOP_JL:
    case IOP_JNB:
    case IOP_JGE:
    case IOP_JBE:
    case IOP_JLE:
    case IOP_JA:
    case IOP_JG:
    case IOP_IDIV:
    case IOP_IMOD:
    case IOP_SETE:
    case IOP_SETNE:
    case IOP_SETG:
    case IOP_SETGE:
    case IOP_SETL:
    case IOP_SETLE:
    case IOP_SETA:
    case IOP_SETNB:
    case IOP_SETB:
    case IOP_SETBE:
    case IOP_LARRAY:
    case IOP_RARRAY:
            return 2;
    case IOP_FUNC:
    case IOP_RETURN:
    case IOP_PARAM:
    case IOP_ASSIGN:
    case IOP_NOT:
    case IOP_UNARY_MINUS:
    case IOP_COERCE:
    case IOP_JNZ:
    case IOP_JZ:
        return 1;
    default:
        return 0;
    }
}

bool iop_has_result(IOperator op) {
    switch (op) {
    case IOP_FUNCCALL:
    case IOP_ASSIGN:
    case IOP_LARRAY:
    case IOP_RARRAY:
    case IOP_ADD:
    case IOP_SUB:
    case IOP_MUL:
    case IOP_DIV:
    case IOP_MOD:
    case IOP_AND:
    case IOP_OR:
    case IOP_NOT:
    case IOP_UNARY_MINUS:
    case IOP_COERCE:
    case IOP_JE:
    case IOP_JNE:
    case IOP_JB:
    case IOP_JL:
    case IOP_JNB:
    case IOP_JGE:
    case IOP_JBE:
    case IOP_JLE:
    case IOP_JA:
    case IOP_JG:
    case IOP_JNZ:
    case IOP_JZ:
    case IOP_IDIV:
    case IOP_IMOD:
    case IOP_SETE:
    case IOP_SETNE:
    case IOP_SETG:
    case IOP_SETGE:
    case IOP_SETL:
    case IOP_SETLE:
    case IOP_SETA:
    case IOP_SETNB:
    case IOP_SETB:
    case IOP_SETBE:
        return true;
    default:
        return false;
    }
}

bool iop_is_goto(IOperator op) {
    return op == IOP_FUNCCALL || op == IOP_GOTO || op == IOP_JA || op == IOP_JB || op == IOP_JE || op == IOP_JNE
        || op == IOP_JL || op == IOP_JNB || op == IOP_JGE || op == IOP_JBE || op == IOP_JLE || op == IOP_JG
        || op == IOP_JNZ || op == IOP_JZ;
}

bool iop_is_cond_jmp(IOperator op) {
    return op == IOP_JA || op == IOP_JB || op == IOP_JE || op == IOP_JNE || op == IOP_JL || op == IOP_JNB
        || op == IOP_JGE || op == IOP_JBE || op == IOP_JLE || op == IOP_JG || op == IOP_JNZ || op == IOP_JZ;
}

static bool isVariable(const SymbolTable& tab, size_t id) {
    SymbolType st = tab.getSymbol(id)->getSymbolType();
    return st == ST_VARIABLE || st == ST_PARAMETER || st == ST_TEMPVAR;
}

bool iop_reads_op1(const SymbolTable& tab, IStatement* stmt) {
    if (iop_arity(stmt->getOperator()) == 0 || stmt->getOperand1() == nullptr)
        return false;
    if (stmt->getOperand1()->getOperandType() != OT_SYMBOL)
        return false;
    return isVariable(tab, std::dynamic_pointer_cast<SymbolIOperand>(stmt->getOperand1())->getId());
}

// Return whether the statement reads a variable at operand 2
bool iop_reads_op2(const SymbolTable& tab, IStatement* stmt) {
    if (iop_arity(stmt->getOperator()) < 2)
        return false;
    if (stmt->getOperand2()->getOperandType() != OT_SYMBOL)
        return false;
    return isVariable(tab, std::dynamic_pointer_cast<SymbolIOperand>(stmt->getOperand2())->getId());
}

bool iop_writes_result(const SymbolTable& tab, IStatement* stmt) {
    if (!iop_has_result(stmt->getOperator()))
        return false;
    if (stmt->getResult() == nullptr || stmt->getResult()->getOperandType() != OT_SYMBOL)
        return false;

    size_t id = std::dynamic_pointer_cast<SymbolIOperand>(stmt->getResult())->getId();
    if (types::isArray(tab.getSymbol(id)->getReturnType()))
        return false;
    return isVariable(tab, id);
}