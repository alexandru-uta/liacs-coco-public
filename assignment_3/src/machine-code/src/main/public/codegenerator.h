#ifndef COCO_FRAMEWORK_MACHINECODE_CODEGENERATOR
#define COCO_FRAMEWORK_MACHINECODE_CODEGENERATOR

#include "../cpp/allocator/globals/globalsallocator.h"
#include <intermediatecode.h>
#include <memory>
#include <ostream>
#include <symboltable.h>
#include <flowgraph.h>

// Class to generate code for a
class CodeGenerator {
    std::ostream& out;
    GlobalsAllocator globals;

  public:
    // Create a code generator, which writes x86/64 assembly to `out`.
    CodeGenerator(std::ostream& out, SymbolTable& tab): out(out), globals(out, tab) {}


    // Generate the header of the assembly.
    void generate_header();

    // Analyze global variables and generate a data segment.
    void generate_global_decls(SymbolTable& table);

    // Translate the code in `inputCode` into x86/64 assembly. `symbtab` should be the
    // symbol table containing information about symbols appearing in `inputCode`.
    void generate_code(SymbolTable& table, IntermediateCode& inputCode, FlowGraph& graph);

    // Generate the assembly trailer.
    void generate_trailer();
};

#endif
