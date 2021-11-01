#include "icgenerator.h"
#include "visitor/icvisitor.h"
#include <memory>
#include <utility.h>
#include <algorithm>

// Note: use this for assignment 5
void ICGenerator::preprocess(const SyntaxTree& /*tree*/, SymbolTable& /*table*/) {}

// Takes a SyntaxTree and converts it into an IntermediateCode structure.
IntermediateCode ICGenerator::generateIntermediateCode(const SyntaxTree& tree, SymbolTable& table) {
    IntermediateCode icode(logger);
    ICVisitor visitor(table, icode);

    auto ids = table.getFunctions();
    std::sort(ids.begin(), ids.end());
    for (size_t id: ids)
        visitor.visit_function(id, tree.getRoot(id));

    return icode;
}

// Note: use this for assignment 5
void ICGenerator::postprocess(IntermediateCode& /*code*/, SymbolTable& /*table*/) {}
