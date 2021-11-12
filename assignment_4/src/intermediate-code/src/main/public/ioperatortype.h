#ifndef BUILD_INTERMEDIATECODE_IOPERATORTYPES
#define BUILD_INTERMEDIATECODE_IOPERATORTYPES

#include <string>
#include <node.h>
#include <to_string.h>

typedef enum __ioperatortype {
    IOPT_VOID,
    IOPT_BYTE,
    IOPT_WORD,
    IOPT_DOUBLE,
    IOPT_QUAD
} IOperatorType;


namespace util { // Adding `to_string` implementation for IOperatorType, so we can use `util::to_string(ioperatortype)`.
    template<>
    std::string to_string<IOperatorType>(IOperatorType);
    IOperatorType to_iopt(ReturnType);
    size_t get_n_bytes(IOperatorType);
}

#endif
