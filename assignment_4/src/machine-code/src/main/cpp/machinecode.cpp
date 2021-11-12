#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <logger.h>
#include <symboltable.h>
#include <syntax.h>
#include <syntaxtree.h>

#include <intermediate.h>

#include "codegenerator.h"
#include "machinecode.h"

void machinecode::generate(SyntaxTree& tree, SymbolTable& table, Logger& logger, const std::string& inputFilePath, const std::string& outputFilePath, bool no_print) {
    // Phase 1: Lexical analysis & syntaxtree generation
    // Parse input file, filling our syntaxtree and symboltable
    int parseResult = syntax::generate(inputFilePath, tree, table, logger);

    if (!no_print) {
        std::cout << "Parse result: " << parseResult << std::endl;
        std::cout << table;
        tree.doStream(std::cout, 4, &table);
    }

    // Phase 2: Intermediate code generation
    intermediate::Intermediate result = intermediate::generate(tree, table, logger);
    if (!no_print) {
        result.icode.doStream(std::cout, &table);
        std::cout << result.graph;
    }

    // Phase 3: Machine code generation
    std::unique_ptr<std::ostream> strrr;
    if (outputFilePath.empty()) {
        strrr = no_print ? std::make_unique<std::ostream>(NULL_STREAM.rdbuf()) : std::make_unique<std::ostream>(std::cout.rdbuf()) ;
    } else {
        strrr = std::make_unique<std::stringstream>();
    }

    CodeGenerator cg = CodeGenerator(*strrr, table);
    cg.generate_header();
    cg.generate_global_decls(table);
    cg.generate_code(table, result.icode, result.graph);
    cg.generate_trailer();

    if (!outputFilePath.empty()) {
        std::ofstream f(outputFilePath);
        f << strrr->rdbuf() << std::endl;
        f.close();
        if (!no_print)
            std::cout << "Output has been stored at: " << outputFilePath << std::endl;
    }
}

void machinecode::generate(Logger& logger, const std::string& inputFilePath, const std::string& outputFilePath, bool no_print) {
    SyntaxTree tree;
    SymbolTable table;
    generate(tree, table, logger, inputFilePath, outputFilePath, no_print);
}