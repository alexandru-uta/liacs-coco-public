/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAXUTILS_TYPES
#define COCO_FRAMEWORK_SYNTAXUTILS_TYPES

#include <string>
#include <to_string.h>

// Return type of a node
typedef enum returntype {
    RT_UNKNOWN,     // Not (yet) known
    RT_ERROR,       // Error
    RT_VOID,        // Void
    RT_INT,         // Integer
    RT_INT_ARRAY,   // Integer array
    RT_INT8,        // 8 width Integer
    RT_INT8_ARRAY,  // Array of 8 width Integers
    RT_UINT,        // Unsigned
    RT_UINT_ARRAY,  // Unsigned array
    RT_UINT8,       // 8 width Unsigned Integer
    RT_UINT8_ARRAY, // Array of 8 width Integers
    RT_REAL,        // Floating point
    RT_BOOL         // Boolean expression
} ReturnType;

// Type of a symbol
typedef enum symboltype {
    ST_UNKNOWN,   // Unknown (yet)
    ST_ERROR,     // Error
    ST_VARIABLE,  // Variable
    ST_PARAMETER, // Function parameter
    ST_FUNCTION,  // Function
    ST_PROGRAM,   // Program body
    ST_TEMPVAR,   // Temporary variable (not used before intermediate code generation)
    ST_LABEL      // Label (not used before intermediate code generation)
} SymbolType;

// Type of a node in the syntax tree
typedef enum nodetype {
    /* Meta */
    NODE_UNKNOWN, // Unknown (yet)
    NODE_ERROR,   // Error

    /* Statement list
     left child     One of: NODE_ASSIGNMENT, NODE_IF, NODE_WHILE,
                    NODE_FUNCTIONCALL, NODE_STATEMENT_LIST, NODE_RETURN.
     right child    NODE_STATEMENT_LIST, or
                    NODE_EMPTY if no more statements follow. */
    NODE_STATEMENT_LIST,

    /* Assignment
     left child     A NODE_ID that identifies the destination variable
     right child    A subtree representing an expression */
    NODE_ASSIGNMENT,

    /* If statement
     left child     An expression that provides the if condition
     right child    A NODE_IF_TARGETS subtree (if there is an else clause) or
                    a NODE_STATEMENT_LIST subtree (if there's no else) */
    NODE_IF,

    /* Targets of an if-else-statement
     left child     NODE_STATEMENT_LIST subtree that has to be executed when the condition
                    of the parent if-statement is true.
     right child    NODE_STATEMENT_LIST subtree that has to be executed when the condition
                    of the parent if-statement is false, that is, the else
                    part. */
    NODE_IF_TARGETS,

    /* While loop
     left child     An expression that provides the loop condition
     right child    A subtree consisting of statements */
    NODE_WHILE,

    /* Lvariable Array Access
     left child   A NODE_ID to the ArraySymbol of the array
     right child  An expression to the index of the access */
    NODE_LARRAY,

    /* Rvariable Array Access
     left child   A NODE_ID to the ArraySymbol of the array
     right child  An expression to the index of the access */
    NODE_RARRAY,

    /* Return
     child          A subtree representing an expression, or NODE_EMPTY for void return */
    NODE_RETURN,

    /* Subprogram calls
     left child     A NODE_ID that identifies the called function
     right child    A NODE_EXPRLIST that specifies the actual arguments, or
                    NODE_EMPTY if no arguments required. */
    NODE_FUNCTIONCALL,

    /* Expression list
     left child     A subtree representing an expression
     right child    A subtree representing an expression, another
                    NODE_EXPRLIST, or NODE_EMPTY if no more expressions follow
                    in the expression list. */
    NODE_EXPRLIST,

    /* Relational operators
     left child     Left-hand side of the comparison
     right child    Right-hand side of the comparison */
    NODE_REL_EQUAL,    //  == operator
    NODE_REL_LT,       //  < operator
    NODE_REL_GT,       //  > operator
    NODE_REL_LTE,      //  <= operator
    NODE_REL_GTE,      //  >= operator
    NODE_REL_NOTEQUAL, //  != operator

    /* BinaryNode arithmetic & logic operations
     left child     Left-hand side of the operation
     right child    Right-hand side of the operation */
    NODE_ADD,  // + Add
    NODE_SUB,  // - Subtract
    NODE_OR,   // || OR operation
    NODE_MUL,  // * Multiply
    NODE_DIV,  // / Divide
    NODE_IDIV, // // Integer division
    NODE_MOD,  // % Modulo
    NODE_AND,  // && AND operation

    /* Leafs
     these nodes have no child nodes */
    NODE_NUM,   // Constant number
    NODE_ID,    // Identifier
    NODE_EMPTY, // Empty leaf (terminates lists etc.)

    /* UnaryNode nodes
     child          The subtree to which the operation has to be applied */
    NODE_NOT,       // NOT operation
    NODE_SIGNPLUS,  // UnaryNode plus
    NODE_SIGNMINUS, // UnaryNode minus
    NODE_COERCION   // Int to Real coercion
} NodeType;

/* Prototypes */
namespace types {
    ReturnType returnTypetoArrayType(ReturnType returnType);
    ReturnType arrayTypetoReturnType(ReturnType arraytype);
    bool isSigned(ReturnType rt);
    bool isArray(ReturnType rt);
}

namespace util {
    template<>
    std::string to_string<ReturnType>(ReturnType);

    template<>
    std::string to_string<SymbolType>(SymbolType);

    template<>
    std::string to_string<NodeType>(NodeType);
}

#endif
