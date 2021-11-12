#include "intermediatecode.h"
#include <utility>

IntermediateCode::~IntermediateCode() {
    for (auto& statement: statements)
        delete statement;
}

// gets the program name
std::string IntermediateCode::getProgramName() {
    return programName;
}

// sets the program name
void IntermediateCode::setProgramName(std::string name) {
    programName = std::move(name);
}

// Returns the number of statements
unsigned IntermediateCode::getStatementCount() const {
    return statements.size();
}

// get the i-th statement
IStatement* IntermediateCode::getStatement(unsigned i) const {
    if (i >= statements.size()) {
        logger.error(-1) << "[IntermediateCode::getStatement()] Error: invalid parameter i.\n";
        return nullptr;
    }

    return statements[i];
}

// Append a statement
void IntermediateCode::appendStatement(IStatement* stmt) {
    if (!stmt)
        logger.warn(-1) << "[IntermediateCode::appendStatement()] Warning: received nullptr statement.\n";
    statements.push_back(stmt);
}

// Insert a statement before the i-th statement
void IntermediateCode::insertStatement(IStatement* stmt, unsigned i) {
    std::vector<IStatement*>::iterator iter;

    if (i > statements.size()) {
        logger.error(-1) << "[IntermediateCode::insertStatement()] Error: parameter i > statement list size.\n";
        return;
    }

    iter = statements.begin() + i;
    statements.insert(iter, stmt);
}

// Remove the i-th statement
void IntermediateCode::removeStatement(unsigned i) {
    std::vector<IStatement*>::iterator iter;

    if (i >= statements.size()) {
        logger.error(-1) << "[IntermediateCode::removeStatement()] Error: invalid parameter i\n";
        return;
    }

    iter = statements.begin() + i;
    statements.erase(iter);
}