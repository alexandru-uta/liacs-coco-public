#include <iostream>

#include "symbol.h"

const std::string& Symbol::getName() const {
    return name;
}
void Symbol::setName(const std::string& newName) {
    Symbol::name = newName;
}
int Symbol::getLine() const {
    return line;
}
void Symbol::setLine(int newLine) {
    Symbol::line = newLine;
}
ReturnType Symbol::getReturnType() const {
    return returnType;
}
void Symbol::setReturnType(ReturnType newReturnType) {
    Symbol::returnType = newReturnType;
}
SymbolType Symbol::getSymbolType() const {
    return symbolType;
}
void Symbol::setSymbolType(SymbolType newSymbolType) {
    Symbol::symbolType = newSymbolType;
}

ssize_t ArraySymbol::getSize() const {
    return size;
}

void ArraySymbol::setSize(ssize_t new_value) {
    size = new_value;
}
