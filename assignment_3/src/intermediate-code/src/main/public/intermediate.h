#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_ENTRYPOINT
#define COCO_FRAMEWORK_INTERMEDIATECODE_ENTRYPOINT

#include <logger.h>
#include <syntaxtree.h>

#include <utility>
#include "flowgraph.h"

namespace intermediate {
    struct Intermediate {
        IntermediateCode icode;
        FlowGraph graph;

        Intermediate(IntermediateCode icode, FlowGraph graph) : icode(std::move(icode)), graph(std::move(graph)) {};
    };

    Intermediate generate(const SyntaxTree& tree, SymbolTable& table, Logger& logger);
}

#endif
