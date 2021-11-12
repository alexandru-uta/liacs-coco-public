#include "register.h"

// https://stackoverflow.com/questions/20637569/assembly-registers-in-64-bit-architecture
static std::string constructRegister(IOperatorType optype, const std::string& middle, const std::string& end) {
    switch (optype) {
        case IOPT_VOID: return std::string("?");
        case IOPT_BYTE: return std::string(middle + 'l');
        case IOPT_WORD: return std::string(middle + end);
        case IOPT_DOUBLE: return std::string(std::string(1, 'e') + middle + end);
        case IOPT_QUAD: return std::string(std::string(1, 'r') + middle + end);
    }
    return "?";
}

static std::string constructGPRegister(IOperatorType optype, const std::string& prefix) {
    switch (optype) {
        case IOPT_VOID: return std::string("?");
        case IOPT_BYTE: return std::string(prefix + 'b');
        case IOPT_WORD: return std::string(prefix + 'w');
        case IOPT_DOUBLE: return std::string(prefix + 'd');
        case IOPT_QUAD: return prefix;
    }
    return "?";
}

template<>
std::string util::to_string<Register>(Register reg) {
    switch (reg.name) {
        case RN_INVALID: return "invalid";
        case RN_RAX: return constructRegister(reg.type, "a", "x");
        case RN_RBX: return constructRegister(reg.type, "b", "x");
        case RN_RCX: return constructRegister(reg.type, "c", "x");
        case RN_RDX: return constructRegister(reg.type, "d", "x");
        case RN_RSI: return constructRegister(reg.type, "si", "");
        case RN_RDI: return constructRegister(reg.type, "di", "");
        case RN_RBP: return constructRegister(reg.type, "bp", "");
        case RN_RSP: return constructRegister(reg.type, "sp", "");
        case RN_R8: return constructGPRegister(reg.type, "r8");
        case RN_R9: return constructGPRegister(reg.type, "r9");
        case RN_R10: return constructGPRegister(reg.type, "r10");
        case RN_R11: return constructGPRegister(reg.type, "r11");
        case RN_R12: return constructGPRegister(reg.type, "r12");
        case RN_R13: return constructGPRegister(reg.type, "r13");
        case RN_R14: return constructGPRegister(reg.type, "r14");
        case RN_R15: return constructGPRegister(reg.type, "r15");
    }
    return "?";
}

