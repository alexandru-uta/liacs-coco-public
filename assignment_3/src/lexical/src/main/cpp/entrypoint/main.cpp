#include <fstream>
#include <iostream>

#include <lexical.h>
#include <logger.h>
#include <nothingvisitor.h>

#include <tclap/CmdLine.h>

/** Simple main entrypoint. Reads file/stdin, prints rule errors. */
int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("C-minus compiler - Lexical analyzer", ' ', "1.0");

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

        // Phase 1: Lexical analysis
        Logger logger(std::cerr, no_warn ? NULL_STREAM : std::cerr, no_error ? NULL_STREAM : std::cerr);
        NothingVisitor vis(logger);
        // Parse input file and get the result.
        int parseResult = lexical::generate(inputFilePath, vis);

        if (!no_print)
            std::cout << "Parse result: " << parseResult << std::endl;
        return 0;
    } catch (TCLAP::ArgException& e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
}