#include "ioperatortype.h"

template<>
std::string util::to_string<IOperatorType>(IOperatorType op) {
    switch (op) {
        case IOPT_BYTE: return "b";
        case IOPT_WORD: return "w";
        case IOPT_DOUBLE: return "l";
        case IOPT_QUAD: return "q";
        default: return " ";
    }
}
IOperatorType util::to_iopt(ReturnType rt) {
    switch (rt) {
        case RT_INT:
        case RT_UINT:
            return IOPT_DOUBLE;
        case RT_INT8:
        case RT_UINT8:
        case RT_BOOL:
            return IOPT_BYTE;
        case RT_UINT8_ARRAY:
        case RT_INT8_ARRAY:
        case RT_UINT_ARRAY:
        case RT_INT_ARRAY:
            return IOPT_QUAD;
        default: return IOPT_VOID;
    }
}

size_t util::get_n_bytes(IOperatorType type) {
    switch (type) {
        case IOPT_VOID: return 0;
        case IOPT_BYTE: return 1;
        case IOPT_WORD: return 2;
        case IOPT_DOUBLE: return 4;
        case IOPT_QUAD: return 8;
    }
    return 0;
}


