#ifndef ICTESTVISITOR_H
#define ICTESTVISITOR_H 

#include "../../../src/main/cpp/intermediatecode/generator/visitor/icvisitor.h"

class ICTestVisitor : public ICVisitor {
public:
	ICTestVisitor(SymbolTable& symtab, IntermediateCode& icode) : ICVisitor(symtab, icode) {};

	void emulate_function_start(size_t id) { function_stack.push_back(id); }
	void emulate_function_end() { function_stack.pop_back(); }
};

#endif