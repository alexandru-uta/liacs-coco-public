#ifndef COCO_FRAMEWORK_MACHINECODE_ENTRYPOINT
#define COCO_FRAMEWORK_MACHINECODE_ENTRYPOINT

#include <string>
#include <logger.h>
#include <symboltable.h>
#include <syntaxtree.h>

namespace machinecode {
    /**
     * Executes all 4 major compilation stages (lexical, syntax, intermediatecode, machinecode) and produces the final output assembly.
     * @param inputFilePath File input.
     * @param outputFilePath File output. If not specified, output is printed to stdout instead.
     * @param no_print If set, does not print anything.
     */
    void generate(SyntaxTree& tree, SymbolTable& table, Logger& logger, const std::string& inputFilePath, const std::string& outputFilePath="", bool no_print=false);

    /**
     * Exactly like above `generate` function, with default-constructed `SyntaxTree` and `SymbolTable`.
     * @see #generate(SyntaxTree&, SymbolTable&, Logger&, const std::string&, const std::string&, bool no_print);
     */
    void generate(Logger& logger, const std::string& inputFilePath, const std::string& outputFilePath="", bool no_print=false);

}

#endif