#include "ioperator.h"

template<>
std::string util::to_string<IOperator>(IOperator op) {
    switch (op) {
    case IOP_UNKNOWN:
        return "unknown";
    case IOP_LABEL:
        return "LABEL";
    case IOP_GOTO:
        return "GOTO";
    case IOP_PARAM:
        return "PARAM";
    case IOP_FUNC:
        return "FUNC";
    case IOP_RETURN:
        return "RETURN";
    case IOP_FUNCCALL:
        return "CALL FUNC";
    case IOP_ASSIGN:
        return "ASSIGN";
    case IOP_ADD:
        return "ADD";
    case IOP_SUB:
        return "SUB";
    case IOP_MUL:
        return "MUL";
    case IOP_DIV:
        return "DIV";
    case IOP_MOD:
        return "MOD";
    case IOP_AND:
        return "AND";
    case IOP_OR:
        return "OR";
    case IOP_NOT:
        return "NOT";
    case IOP_UNARY_MINUS:
        return "UMINUS";
    case IOP_COERCE:
        return "COERCION";
    case IOP_LARRAY:
        return "LARRAY";
    case IOP_RARRAY:
        return "RARRAY";
    case IOP_JE:
        return "JUMP EQUAL";
    case IOP_JNE:
        return "JUMP NOT EQUAL";
    case IOP_JB:
        return "JUMP BELOW";
    case IOP_JL:
        return "JUMP LESS";
    case IOP_JNB:
        return "JUMP NOT BELOW";
    case IOP_JGE:
        return "JUMP GREATER EQUAL";
    case IOP_JBE:
        return "JUMP BELOW EQUAL";
    case IOP_JLE:
        return "JUMP LESS EQUAL";
    case IOP_JA:
        return "JUMP ABOVE";
    case IOP_JG:
        return "JUMP GREATER";
    case IOP_JNZ:
        return "JUMP TRUE";
    case IOP_JZ:
        return "JUMP FALSE";
    case IOP_IDIV:
        return "SIGNED DIVIDE";
    case IOP_IMOD:
        return "SIGNED MODULO";
    case IOP_SETE:
        return "SET IF EQUAL";
    case IOP_SETNE:
        return "SET IF NOT EQUAL";
    case IOP_SETG:
        return "SET IF GREATER";
    case IOP_SETGE:
        return "SET IF GREATER EQUAL";
    case IOP_SETL:
        return "SET IF LESS";
    case IOP_SETLE:
        return "SET IF LESS EQUAL";
    case IOP_SETA:
        return "SET ABOVE";
    case IOP_SETNB:
        return "SET NOT ABOVE";
    case IOP_SETB:
        return "SET BELOW";
    case IOP_SETBE:
        return "SET BELOW EQUAL";
    }
    return "???";
}
