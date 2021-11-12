#include <fstream>
#include <iostream>

#include <logger.h>
#include <symboltable.h>
#include <syntax.h>
#include <syntaxtree.h>

#include <icgenerator.h>
#include <intermediate.h>
#include <tclap/CmdLine.h>

void printUsage(char* program) {
    std::cout << "Usage: " << program << "[OPTIONS] [FILENAME]" << std::endl;
    std::cout << "\t[OPTIONS]: " << std::endl;
    std::cout << "\t\t-h: display this message and exits" << std::endl;
    std::cout << "\t\t-f[FILENAME]: name of the file to parse. If no filename is provided, program will read from stdin";
    std::cout << std::endl;
}

/** Simple main entrypoint. Reads file/stdin, prints generated intermediate code. */
int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("C-minus compiler - Intermediate Code generator", ' ', "1.0");

    try {
        TCLAP::ValueArg<std::string> filenameArg("f", "file", "Path to source file.", true, "", "string", cmd);
        TCLAP::SwitchArg quietSwitch("q", "quiet", "Do not print warnings.", cmd, false);
        cmd.parse(argc, argv);

        bool quiet = quietSwitch.getValue();
        const std::string& filepath = filenameArg.getValue();

        // Phase 1: Lexical analysis & syntaxtree generation
        Logger logger(std::cerr, quiet ? NULL_STREAM : std::cerr, std::cerr);
        SymbolTable table;
        SyntaxTree tree;

        // Parse input file, filling our syntaxtree and symboltable
        int parseResult = syntax::generate(filepath, tree, table, logger);

        std::cout << "Parse result: " << parseResult << std::endl;
        std::cout << table;
        tree.doStream(std::cout, 4, &table);

        // Phase 2: Intermediate code generation
        intermediate::Intermediate result = intermediate::generate(tree, table, logger);
        result.icode.doStream(std::cout, &table);
        std::cout << result.graph;
        return 0;
    } catch (TCLAP::ArgException& e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
}