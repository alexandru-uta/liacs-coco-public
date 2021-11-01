#include "scope.h"
#include <limits>
#include <iostream>

bool Scope::addSymbol(const std::string& name, size_t sym_id) {
    return symbols.insert({name, sym_id}).second;
}

size_t Scope::getSymbolId(const std::string& sym_name) {
    auto item = symbols.find(sym_name);
    if (item == symbols.end())
        return std::numeric_limits<size_t>::max();
    return (*item).second;
}

size_t Scope::getNumberOfSymbols() const { return symbols.size(); }
