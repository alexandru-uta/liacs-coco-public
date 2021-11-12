#include <iostream>

#include "machinecode.h"
#include <logger.h>
#include <tclap/CmdLine.h>


/** Simple main entrypoint. Reads file/stdin, prints generated intermediate code. */
int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("C-minus compiler - Machine Code generator", ' ', "1.0");

    try {
        TCLAP::ValueArg<std::string> inputFilenameArg("f", "file", "Path to source file.", true, "", "string", cmd);
        TCLAP::ValueArg<std::string> outputFilenameArg("o", "output", "Path to destination file.", false, "", "string", cmd);
        TCLAP::SwitchArg noWarningSwitch("w", "no-warn", "Do not print warnings.", cmd, false);
        TCLAP::SwitchArg noPrintSwitch("p", "no-print", "Do not print output.", cmd, false);
        cmd.parse(argc, argv);

        bool no_warn = noWarningSwitch.getValue();
        bool no_print = noPrintSwitch.getValue();
        const std::string& inputFilePath = inputFilenameArg.getValue();
        const std::string& outputFilePath = outputFilenameArg.getValue();

        Logger logger = Logger(std::cerr, no_warn ? NULL_STREAM : std::cerr, std::cerr);
        machinecode::generate(logger, inputFilePath, outputFilePath, no_print);
        return 0;
    } catch (TCLAP::ArgException& e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
}