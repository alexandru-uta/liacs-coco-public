#include <fstream>
#include <iostream>

#include <logger.h>
#include <symboltable.h>
#include <syntax.h>
#include <syntaxtree.h>

#include <tclap/CmdLine.h>


/** Simple main entrypoint. Reads file/stdin, prints generated syntax tree. */
int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("C-minus compiler - Syntax Tree generator", ' ', "1.0");

    try {
        TCLAP::ValueArg<std::string> inputFilenameArg("f", "file", "Path to source file.", true, "", "string", cmd);
        TCLAP::SwitchArg noWarningSwitch("w", "no-warn", "Do not print warnings.", cmd, false);
        TCLAP::SwitchArg noErrorSwitch("e", "no-error", "Do not print errors.", cmd, false);
        TCLAP::SwitchArg noPrintSwitch("p", "no-print", "Do not print output.", cmd, false);
        cmd.parse(argc, argv);

        bool no_warn = noWarningSwitch.getValue();
        bool no_error = noErrorSwitch.getValue();
        bool no_print = noPrintSwitch.getValue();
        const std::string& inputFilePath = inputFilenameArg.getValue();

        // Phase 1: Lexical analysis & syntaxtree generation
        Logger logger(std::cerr, no_warn ? NULL_STREAM : std::cerr, no_error ? NULL_STREAM : std::cerr);
        SymbolTable table;
        SyntaxTree tree;

        // Parse input file, filling our syntaxtree and symboltable
        int parseResult = syntax::generate(inputFilePath, tree, table, logger);

        if (!no_print) {
            std::cout << "Parse result: " << parseResult << std::endl;
            std::cout << table;
            tree.doStream(std::cout, 4, &table);
        }
        return 0;
    } catch (TCLAP::ArgException& e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
}