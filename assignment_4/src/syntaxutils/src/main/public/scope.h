/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAXUTILS_SCOPE
#define COCO_FRAMEWORK_SYNTAXUTILS_SCOPE

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "symbol.h"

// Class representing a scope
class Scope {
    public:
    Scope() = default;
    ~Scope() = default;

    /**
     * Adds the given symbol to this scope
     * @param name name of the symbol
     * @param sym_id sym_id of the symbol
     * @return `true` on success, `false` if the symbol could not be added (already exists)
     */
    bool addSymbol(const std::string& name, size_t sym_id);

    /**
     * Returns the id of the symbol identified by <sym_name> in this scope.
     * @param sym_name name of the symbol
     * @return the found id or std::numeric_limits<size_t>::max() on error
     */
    size_t getSymbolId(const std::string& sym_name);

    // Returns the number of symbols in this scope
    size_t getNumberOfSymbols() const;

    private:
    // Note: name of symbols should be unique within each scope
    std::unordered_map<std::string, size_t> symbols;
};
#endif
