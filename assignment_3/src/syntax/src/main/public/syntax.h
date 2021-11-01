/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAX_ENTRYPOINT
#define COCO_FRAMEWORK_SYNTAX_ENTRYPOINT

#include <logger.h>
#include <symboltable.h>
#include <syntaxtree.h>
namespace syntax {
    /**
     * Uses Flex/Bison generated files to parse data.
     * @return `yyparse` exit code.
     */
    int generate(FILE* file, SyntaxTree&, SymbolTable&, Logger&);

    /**
     * @param filename File to be read in.
     * @return `yyparse` exit code.
     */
    int generate(const std::string& filename, SyntaxTree& tree, SymbolTable& table, Logger& logger);
}

#endif
