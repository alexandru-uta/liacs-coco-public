#ifndef COCO_FRAMEWORK_MACHINECODE_MACHINECODEUTIL
#define COCO_FRAMEWORK_MACHINECODE_MACHINECODEUTIL

#include <iostream>
#include <string>
#include <sstream>

#include <ghc/filesystem.h>
#include <subprocess.hpp>


namespace test {
    /**
     * Builds a valid assembly output path for a c-minus test.
     * e.g, if provided with path `test/c-minus/programs/gcd.c`, this will output `/tmp/gcd.s` on Linux systems.
     * @param testpath Path to test file.
     * @return assembly output path for compiled c-minus testfile.
     */
    inline std::string assemblypath_for(const std::string& testpath) {
        const auto tmp_dir = ghc::filesystem::temp_directory_path();
        const ghc::filesystem::path p(testpath, ghc::filesystem::path::auto_format);
        const auto stem = p.stem();
        auto outpath = tmp_dir / stem;
        return outpath.replace_extension(".s").string();
    }

    /**
     * Builds a valid executable output path for an assembly path.
     * e.g, if provided with path `/tmp/gcd.s`, this will output `/tmp/gcd` on Linux systems.
     * @param assemblypath Path to assembly file.
     * @return executable output path.
     */
    inline std::string executablepath_for(const std::string& assemblypath) {
        const ghc::filesystem::path p(assemblypath, ghc::filesystem::path::auto_format);
        return p.parent_path() / p.stem();
    }

    /**
     * Compiles a given assembly file, and outputs the binary at given executable path.
     * @param assemblypath Path to existing assembly file.
     * @param executablepath Path to store created executable file.
     * @return Compiler exitcode.
     */
    inline int compile(const std::string& assemblypath, const std::string& executablepath) {
        const ghc::filesystem::path p(assemblypath, ghc::filesystem::path::auto_format);
        std::stringstream ss;
        ss << "cc " << assemblypath << " -o " << executablepath;
        const auto command = ss.str();
        // Students: If above command does not work for your OS, implement your own call here to compile the file pointed to by `assemblypath`, generating an executable at `executablepath`.
        // Our implementation creates "cc {assemblypath} -o {executablepath}", which works on university machines, and should work on any Linux/MacOS system.
        return std::system(command.c_str());
    }

    /**
     * Compiles a given assembly file, and outputs the binary at the same location, without the extension, i.e. `/tmp/gcd.s` --> `/tmp/gcd`.
     * @param assemblypath Path to existing assembly file.
     * @return Compiler exitcode
     * @see #compile(const std::string&, const std::string&)
     */
    inline int compile(const std::string& assemblypath) {
        return compile(assemblypath, executablepath_for(assemblypath));
    }

    /**
     * Executes provided executable, with optional provided standard input.
     * @param executablepath Path to executable.
     * @param cin If set, provides given cin.
     * @return subprocess::CompletedProcess, contain useful information, such as captured standard output, exitcode.
     */
    inline subprocess::CompletedProcess execute(const std::string& executablepath, const std::string& cin="", const double timeout = -1) {
        subprocess::CompletedProcess process = subprocess::run({executablepath},subprocess::RunBuilder().cin(cin).cout(subprocess::PipeOption::pipe).timeout(timeout));
        // subprocess::CompletedProcess useful properties:
        //      process.cout
        //      process.returncode
        return process;
    }
}

#endif