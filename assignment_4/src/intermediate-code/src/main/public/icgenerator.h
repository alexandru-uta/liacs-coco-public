#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_ICGENERATOR
#define COCO_FRAMEWORK_INTERMEDIATECODE_ICGENERATOR

#include "intermediatecode.h"
#include <logger.h>
#include <symboltable.h>
#include <syntaxtree.h>

// This class handles the intermediate code generation. Extend it to your own needs.
class ICGenerator {
    protected:
    Logger& logger;
    public:
    explicit ICGenerator(Logger& logger): logger(logger) {}

    // Preprocesses the syntax tree; this method is called before GenerateIntermediateCode() if optimizations are enabled.
    void preprocess(const SyntaxTree& tree, SymbolTable& table);

    // Takes a SyntaxTree and converts it into an IntermediateCode structure.
    IntermediateCode generateIntermediateCode(const SyntaxTree& tree, SymbolTable& table);

    // Postprocesses the intermediate code; this method is called after GenerateIntermediateCode() if optimizations are enabled.
    void postprocess(IntermediateCode& code, SymbolTable& table);
};

#endif
