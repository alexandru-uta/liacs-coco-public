#ifndef COCO_FRAMEWORK_MACHINECODE_REGISTER
#define COCO_FRAMEWORK_MACHINECODE_REGISTER

#include <functional>
#include <iostream>
#include <types.h>
#include <utility>
#include <ioperatortype.h>

// Note: (https://stackoverflow.com/questions/15191178/how-do-ax-ah-al-map-onto-eax)
// writing into AL, AH, AX, leaves other bytes unmodified, writing EAX, zero-extends into RAX
typedef enum registername {
    // Name, convention
    RN_INVALID, // invalid register
    RN_RAX, // return values, caller-save
    RN_RBX, // storing data related to RAX computation, callee-save
    RN_RCX, // counter, fourth argument, caller-save
    RN_RDX, // third argument, caller-save
    RN_RSI, // second argument, caller-save
    RN_RDI, // first argument, caller-save
    RN_RBP, // base pointer, callee-save
    RN_RSP, // stack pointer, caller-save
    RN_R8, // fifth argument, caller-save
    RN_R9, // sixth argument, caller-save
    RN_R10, // caller-save
    RN_R11, // caller-save
    RN_R12, // callee-save
    RN_R13, // callee-save
    RN_R14, // callee-save
    RN_R15, // callee-save
} RegisterName;


struct Register {
    IOperatorType type;
    RegisterName name;
};

namespace util { // Adding `to_string` implementation for Register, so we can use `util::to_string(register)`.
    template<>
    std::string to_string<Register>(Register);
}

inline std::ostream& operator<<(std::ostream& os, const Register& reg) {
    return os << "%" << util::to_string(reg);
}

inline bool operator<(const Register& a, const Register& b) {
    return std::make_tuple(a.type, a.name) < std::make_tuple(b.type, b.name);
}

inline bool operator==(const Register& a, const Register& b) {
    return std::make_tuple(a.type, a.name) == std::make_tuple(b.type, b.name);
}


#endif
