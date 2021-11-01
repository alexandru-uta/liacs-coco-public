#include "frontendbuilder.h"
#include <iostream>

FrontendBuilder::~FrontendBuilder() {
    for (auto& scope: scope_stack)
        delete scope;
}

void FrontendBuilder::enterNewScope() {
    //TODO: implement me
}

void FrontendBuilder::exitCurrentScope() {
    //TODO: implement me
}

size_t FrontendBuilder::addSymbol(Symbol* symbol) {
    //TODO: implement me
    return std::numeric_limits<size_t>::max();
}

size_t FrontendBuilder::enterFunction(Symbol* program) {
    //TODO: implement me
    return std::numeric_limits<size_t>::max();
}

void FrontendBuilder::leaveFunction() {
    //TODO: implement me
}

size_t FrontendBuilder::getId(const std::string& name) {
    //TODO: implement me
    return std::numeric_limits<size_t>::max();
}

Symbol* FrontendBuilder::getSymbol(size_t id) {
    return table.getSymbol(id);
}

Symbol* FrontendBuilder::getSymbol(const std::string& name) {
    size_t id = getId(name);
    if (id == std::numeric_limits<size_t>::max())
        return nullptr;
    return table.getSymbol(id);
}

Symbol* FrontendBuilder::getCurrentFunction() {
    return table.getSymbol(current_function);
}

bool FrontendBuilder::attachRoot(const std::string& name, Node* new_root) {
    //TODO: implement me
    return false;
}

bool FrontendBuilder::getParameters(const std::string& name, std::vector<Symbol*>& parameters) {
    return table.getParameters(getId(name), parameters);
}