/*
 * types.cc - Various type definitions
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 */

#include "types.h"

ReturnType types::returnTypetoArrayType(ReturnType returnType) {
    switch (returnType) {
    case RT_INT:
        return RT_INT_ARRAY;
    case RT_UINT:
        return RT_UINT_ARRAY;
    case RT_INT8:
        return RT_INT8_ARRAY;
    case RT_UINT8:
        return RT_UINT8_ARRAY;
    default:
        return RT_ERROR;
    }
}

ReturnType types::arrayTypetoReturnType(ReturnType arraytype) {
    switch (arraytype) {
    case RT_INT_ARRAY:
        return RT_INT;
    case RT_UINT_ARRAY:
        return RT_UINT;
    case RT_INT8_ARRAY:
        return RT_INT8;
    case RT_UINT8_ARRAY:
        return RT_UINT8;
    default:
        return RT_ERROR;
    }
}

template<>
std::string util::to_string<ReturnType>(ReturnType returntype) {
    switch (returntype) {
    case RT_UNKNOWN:
        return "unknown";
    case RT_ERROR:
        return "error";
    case RT_VOID:
        return "void";
    case RT_INT:
        return "int";
    case RT_INT_ARRAY:
        return "int array";
    case RT_INT8:
        return "int8";
    case RT_INT8_ARRAY:
        return "int8 array";
    case RT_UINT:
        return "unsigned";
    case RT_UINT_ARRAY:
        return "unsigned array";
    case RT_UINT8:
        return "unsigned_8";
    case RT_UINT8_ARRAY:
        return "unsigned_8 array";
    case RT_REAL:
        return "real";
    case RT_BOOL:
        return "bool";
    default:
        return "??";
    };
}

template<>
std::string util::to_string<SymbolType>(SymbolType symboltype) {
    switch (symboltype) {
    case ST_UNKNOWN:
        return "unknown";
    case ST_ERROR:
        return "error";
    case ST_VARIABLE:
        return "variable";
    case ST_PARAMETER:
        return "parameter";
    case ST_FUNCTION:
        return "function";
    case ST_PROGRAM:
        return "program";
    case ST_TEMPVAR:
        return "temp var";
    case ST_LABEL:
        return "label";
    default:
        return "??";
    };
}

template<>
std::string util::to_string<NodeType>(NodeType nodetype) {
    switch (nodetype) {
    case NODE_UNKNOWN:
        return "Unknown";
    case NODE_ERROR:
        return "Error";
    case NODE_STATEMENT_LIST:
        return "Statement list";
    case NODE_ASSIGNMENT:
        return "Assignment";
    case NODE_IF:
        return "If";
    case NODE_IF_TARGETS:
        return "If Targets";
    case NODE_WHILE:
        return "While";
    case NODE_LARRAY:
        return "Lvariable Array";
    case NODE_RARRAY:
        return "Rvariable Array";
    case NODE_RETURN:
        return "Return";
    case NODE_FUNCTIONCALL:
        return "Function call";
    case NODE_EXPRLIST:
        return "Expression list";
    case NODE_REL_EQUAL:
        return "Equality";
    case NODE_REL_LT:
        return "Less than";
    case NODE_REL_GT:
        return "Greater than";
    case NODE_REL_LTE:
        return "Less than or equal to";
    case NODE_REL_GTE:
        return "Greater than or equal to";
    case NODE_REL_NOTEQUAL:
        return "Not equal to";
    case NODE_ADD:
        return "Addition";
    case NODE_SUB:
        return "Substraction";
    case NODE_OR:
        return "Bitwise OR";
    case NODE_MUL:
        return "Multiply";
    case NODE_DIV:
        return "Divide";
    case NODE_IDIV:
        return "Integer division";
    case NODE_MOD:
        return "Modulo";
    case NODE_AND:
        return "Bitwise AND";
    case NODE_NUM:
        return "Constant Number";
    case NODE_NOT:
        return "Bitwise NOT";
    case NODE_ID:
        return "Identifier";
    case NODE_EMPTY:
        return "empty leaf";
    case NODE_SIGNPLUS:
        return "UnaryNode plus";
    case NODE_SIGNMINUS:
        return "UnaryNode minus";
    case NODE_COERCION:
        return "Coercion";
    default:
        return "??";
    }
}

bool types::isSigned(ReturnType rt) {
    return rt == RT_INT || rt == RT_INT8 || rt == RT_INT_ARRAY || rt == RT_INT8_ARRAY;
}

bool types::isArray(ReturnType rt) {
    return rt == RT_INT_ARRAY || rt == RT_INT8_ARRAY || rt == RT_UINT_ARRAY || rt == RT_UINT8_ARRAY;
}
