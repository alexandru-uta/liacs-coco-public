/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAXUTILS_SYMBOLTABLE
#define COCO_FRAMEWORK_SYNTAXUTILS_SYMBOLTABLE

#include <iomanip>
#include <string>
#include <vector>

#include "node.h"
#include "scope.h"
#include "symbol.h"

class SymbolTable {
    public:
    SymbolTable() = default;
    ~SymbolTable();

    /**
     * Adds a (non-function) symbol corresponding to a function to the table.
     * Does nothing if function does not exist.
     * @param symbol the symbol to add.
     * @param function the identifier of the function to which it belongs or 0 for global.
     * @return the identifier of the added symbol or std::numeric_limits<size_t>::max() if function does not exist.
     */
    size_t addSymbol(Symbol* symbol, size_t function);

    /**
     * Adds a function symbol to the table.
     * @param symbol the function symbol to add.
     * @param variables (optional) a vector of Symbol* corresponding to the variables of the function.
     * @param parameters (optional) a vector of Symbol* corresponding to the parameters of the function.
     * @return the identifier of the added symbol.
     */
    size_t addFunction(Symbol* symbol, const std::vector<Symbol*>& variables = {}, const std::vector<Symbol*>& parameters = {});

    /**
     * Find a symbol by its id.
     * @param id id of the symbol to find.
     * @return Returns found symbol on success, `nullptr` if the symbol cannot be found.
     */
    Symbol* getSymbol(size_t id) const;

    /**
     * @return all global (non-function) symbols.
     */
    std::unordered_map<size_t, Symbol*> getGlobals() const;

    /**
     * @param id the identifier of a symbol.
     * @return whether the provided symbol is a global.
     */
    bool isGlobal(size_t id) const;

    /**
     * Get all functions.
     * @return a vector to the function ids.
     */
    std::vector<size_t> getFunctions() const;

    /**
     * pushes all function variables into the given vector.
     * @param Function ID to push variables for.
     * @param variables Vector to place found function variables into.
     * @return `true` if all actions succeeded, `false` otherwise.
     */
    bool getVariables(size_t id, std::vector<size_t>& variables) const;

    /**
     * pushes all function variables into the given vector.
     * @see SymbolTable::getVariables(size_t, std::vector<size_t>&)
     */
    bool getVariables(size_t id, std::vector<Symbol*>& variables) const;

    /**
     * pushes all function parameters into the given vector.
     * @param Function ID to push parameters for.
     * @param variables Vector to place found function parameters into.
     * @return `true` if all actions succeeded, `false` otherwise.
     */
    bool getParameters(size_t id, std::vector<size_t>& parameters) const;

    /**
     * pushes all function parameters into the given vector.
     * @see SymbolTable::getVariables(size_t, std::vector<size_t>&)
     */
    bool getParameters(size_t id, std::vector<Symbol*>& parameters) const;

    size_t addTempvar(ReturnType rt, const std::string& name, size_t func_id);
    size_t addLabel(ReturnType rt, const std::string& name, size_t func_id);

    inline friend std::ostream& operator<<(std::ostream& stream, const SymbolTable& table) {
        stream << "===== Symbol table dump =====\n";
        if (table.getGlobals().size() > 0) {
            stream << "----- Global symbols -----\n";
            stream << std::left << std::setw(4) << "id" << std::setw(6) << "Line" << std::setw(16) << "name" << std::setw(12) << "ReturnType"
                   << "SymbolType\n";
            for (const auto& global: table.getGlobals()) {
                const auto& id = global.first;
                const auto& globalsym = table.getSymbol(id);
                if (globalsym)
                    stream << std::setw(4) << id << *globalsym << '\n';
                else
                    stream << std::setw(4) << id << "Global Variable not found!\n";
            }
        }
        stream << "----- Function symbols -----\n";
        for (const auto& function: table.functions) {
            const auto& funsym = table.getSymbol(function.first);
            stream << std::left << "FUNCTION: " << std::setw(16) << funsym->getName() << "id: " << std::setw(4) << function.first << "line: " << funsym->getLine() << '\n';
            if (function.second.parameters.empty() && function.second.variables.empty())
                continue;
            stream << std::setw(4) << "id" << std::setw(6) << "Line" << std::setw(16) << "name" << std::setw(12) << "ReturnType"
                   << "SymbolType\n";
            for (const auto& id: function.second.parameters) {
                const auto& paramsym = table.getSymbol(id);
                if (paramsym)
                    stream << std::left << std::setw(4) << id << *paramsym << '\n';
                else
                    stream << std::left << std::setw(4) << id << "Parameter not found!\n";
            }
            for (const auto& id: function.second.variables) {
                const auto& varsym = table.getSymbol(id);
                if (varsym)
                    stream << std::setw(4) << id << *varsym << '\n';
                else
                    stream << std::setw(4) << id << "Variable not found!\n";
            }
        }
        stream << "===== End of symbol table dump =====\n";
        return stream;
    }

    private:
    friend class FrontendBuilder;

    // Struct to maintain some metadata of Symbols
    struct SymbolInfo {
        size_t id = 0;
        // id of function to which this variable belongs
        // if symbol is global this should be 0
        size_t func_id = 0;
        Symbol* symbol = nullptr;

        inline friend std::ostream& operator<<(std::ostream& stream, const SymbolInfo& sym_info) {
            stream << "--- Symbol: id " << sym_info.id << ", function: " << sym_info.func_id << std::endl;
            stream << sym_info.symbol;
            return stream;
        }
    };

    // Struct to maintain some metadata of Functions
    struct FunctionInfo {
        // Ids of variables in the scope of the Function
        // includes all non-parameter symbol types
        std::vector<size_t> variables;
        // Ids of parameters in the scope of the Function
        std::vector<size_t> parameters;
    };

    // Map from symbol-id to its symbolInfo
    std::unordered_map<size_t, SymbolInfo> symbols;
    // Map from function-id to all its variables
    std::unordered_map<size_t, FunctionInfo> functions;

    size_t addSymbol(SymbolType st, ReturnType rt, const std::string& name, size_t func_id);
};

#endif
