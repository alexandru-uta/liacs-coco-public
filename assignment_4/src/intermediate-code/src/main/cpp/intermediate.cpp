#include <logger.h>
#include <syntaxtree.h>

#include "icgenerator.h"
#include "intermediatecode.h"

#include "intermediate.h"

intermediate::Intermediate intermediate::generate(const SyntaxTree& tree, SymbolTable& table, Logger& logger) {
    ICGenerator generator(logger);
    generator.preprocess(tree, table);

    IntermediateCode icode = generator.generateIntermediateCode(tree, table);
    FlowGraph graph(table, icode, logger);

    generator.postprocess(icode, table);
    return {std::move(icode), graph};
}
