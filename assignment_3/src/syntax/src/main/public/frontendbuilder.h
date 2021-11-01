/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAX_FRONTENDBUILDER
#define COCO_FRAMEWORK_SYNTAX_FRONTENDBUILDER

#include "node.h"
#include "symbol.h"
#include "symboltable.h"
#include "syntaxtree.h"
class FrontendBuilder {
    public:
    explicit FrontendBuilder(SymbolTable& table, SyntaxTree& tree) : table(table), tree(tree){};
    ~FrontendBuilder();

    // creates a new scope and sets this as the current scope
    void enterNewScope();

    // exits the current scope
    void exitCurrentScope();

    // adds a new symbol to the current scope
    // use for both parameters as variables
    // returns symbol identifier, or std::numeric_limits<size_t>::max() on error
    size_t addSymbol(Symbol* symbol);

    /**
     * adds a new function and sets this as the current function
     * @param program
     * @return function identifier or std::numeric_limits<size_t>::max() on error
     */
    size_t enterFunction(Symbol* program);

    // leaves the current function
    void leaveFunction();

    // find id of symbol identified by name
    size_t getId(const std::string& name);

    /**
     * Find a symbol in the current scope. Looks in the parent scope if it cannot be found in the current scope.
     * @param id ID of the symbol to find.
     * @return Returns found symbol on success, `nullptr` if the symbol cannot be found.
     */
    Symbol* getSymbol(size_t id);

    /**
     * Find a symbol in the current scope. Looks in the parent scope if it cannot be found in the current scope.
     * @param name Name of the symbol to find.
     * @return Returns found symbol on success, `nullptr` if the symbol cannot be found.
     */
    Symbol* getSymbol(const std::string& name);

    /**
     * Retrieves the current function
     * @return Returns the symbol of the current function
     */
    Symbol* getCurrentFunction();

    // attach new root to function
    bool attachRoot(const std::string& name, Node* new_root);

    // pushes all function parameters into the given vector
    // returns whether actions succeeded
    bool getParameters(const std::string& name, std::vector<Symbol*>& parameters);

    private:
    SymbolTable& table;
    SyntaxTree& tree;

    // Stack to keep track of current scope
    std::vector<Scope*> scope_stack = {new Scope()};
    // Identifier of the current function
    size_t current_function = 0;
};

#endif
