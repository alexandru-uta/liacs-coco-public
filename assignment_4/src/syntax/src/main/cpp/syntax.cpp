#include "syntax.h"
#include "visitor/syntaxvisitor.h"
#include <lexical.h>

int syntax::generate(FILE* file, SyntaxTree& tree, SymbolTable& table, Logger& logger) {
    SyntaxVisitor vis(logger, table, tree);
    return lexical::generate(file, vis);
}

int syntax::generate(const std::string& filename, SyntaxTree& tree, SymbolTable& table, Logger& logger) {
    SyntaxVisitor vis(logger, table, tree);
    return lexical::generate(filename, vis);
}