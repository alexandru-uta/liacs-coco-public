#include <string>
#include <vector>

#include "scope.h"
#include "symbol.h"

#include "symboltable.h"

SymbolTable::~SymbolTable() {
    for (auto& symbol: symbols)
        delete symbol.second.symbol;
}

size_t SymbolTable::addSymbol(Symbol* symbol, size_t function) {
    // TODO: implement me
    return std::numeric_limits<size_t>::max();
}

size_t SymbolTable::addFunction(Symbol* symbol, const std::vector<Symbol*> &variables, const std::vector<Symbol*> &parameters) {
    // TODO: implement me
    return std::numeric_limits<size_t>::max();
}

Symbol* SymbolTable::getSymbol(size_t id) const {
    auto item = symbols.find(id);
    if (item == symbols.end())
        return nullptr;
    return item->second.symbol;
}

std::unordered_map<size_t, Symbol*>  SymbolTable::getGlobals() const {
    //TODO: implement me
    std::unordered_map<size_t, Symbol*> globals;
    return globals;
}

bool SymbolTable::isGlobal(size_t id) const {
    auto it = symbols.find(id);
    if (it == symbols.end())
        return false;
    return it->second.func_id == 0;
}

bool SymbolTable::getVariables(size_t func_id, std::vector<size_t>& variables) const {
    auto item = functions.find(func_id);
    if (item == functions.end())
        return false;
    variables = item->second.variables;
    return true;
}

bool SymbolTable::getVariables(size_t func_id, std::vector<Symbol*>& variables) const {
    std::vector<size_t> ids;
    if (!getVariables(func_id, ids))
        return false;

    for (size_t id: ids) {
        auto element = symbols.find(id);
        if (element == symbols.end())
            return false;
        variables.push_back(element->second.symbol);
    }
    return true;
}

bool SymbolTable::getParameters(size_t func_id, std::vector<size_t>& parameters) const {
    auto item = functions.find(func_id);
    if (item == functions.end())
        return false;
    parameters = item->second.parameters;
    return true;
}

bool SymbolTable::getParameters(size_t func_id, std::vector<Symbol*>& parameters) const {
    std::vector<size_t> ids;
    if (!getParameters(func_id, ids))
        return false;
    for (size_t id: ids) {
        auto element = symbols.find(id);
        if (element == symbols.end())
            return false;
        parameters.push_back(element->second.symbol);
    }
    return true;
}

size_t SymbolTable::addTempvar(ReturnType rt, const std::string& name, size_t func_id) {
    return addSymbol(ST_TEMPVAR, rt, name, func_id);
}

size_t SymbolTable::addLabel(ReturnType rt, const std::string& name, size_t func_id) {
    return addSymbol(ST_LABEL, rt, name, func_id);
}

size_t SymbolTable::addSymbol(SymbolType st, ReturnType rt, const std::string& name, size_t func_id) {
    //TODO: implement me
    return std::numeric_limits<size_t>::max();
}
std::vector<size_t> SymbolTable::getFunctions() const {
    std::vector<size_t> keys;
    for (auto& func: functions) {
        keys.push_back(func.first);
    }
    return keys;
}