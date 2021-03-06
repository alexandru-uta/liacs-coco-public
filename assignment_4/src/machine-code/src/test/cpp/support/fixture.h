#ifndef COCO_FRAMEWORK_TEST_MACHINECODE_FIXTURE
#define COCO_FRAMEWORK_TEST_MACHINECODE_FIXTURE

#include "machinecodeutil.h"
#include "gtest/gtest.h"

#include <test.h>


#include <syntax.h>
#include <logger.h>
#include <symboltable.h>
#include <syntaxtree.h>
#include <intermediate.h>
#include <codegenerator.h>

#include <ostream>
#include <machinecode.h>
#include <ghc/filesystem.h>


class MachineCodeTest : public CapturingLoggingTest {
protected:
    MachineCodeTest(std::ostream& infostream, std::ostream& warnstream, std::ostream& errorstream, std::ostream& out) : logger(infostream, warnstream, errorstream), out(out) {}

    Logger logger;
    SymbolTable table{};
    SyntaxTree tree{};
    std::ostream& out;
};

class DynamicMachineCodeTest : public MachineCodeTest, public IOTest {
protected:
    explicit DynamicMachineCodeTest(std::ostream& infostream, std::ostream& warnstream, std::ostream& errorstream, std::ostream& out, const std::string& exepath, const std::string& testpath) : MachineCodeTest(infostream, warnstream, errorstream, out), IOTest(), exepath(exepath), testpath(testpath){};

    /**
     * Simple function to test whether our system is segfault-free.
     * If we execute this function from another thread and that thread segfaults, its exitcode is never 0.
     * By checking the thread exitcode, we know whether we segfaulted.
     */
    int exitTest() {
        std::stringstream ss;
        ss << exepath << " --no-print -f \"" << testpath << "\"";
        int exitCode = std::system(ss.str().c_str());
        return exitCode;
    }

    const std::string& exepath; // Path to the file we are currently testing.
    const std::string& testpath; // Path to the file we are currently testing.
};

class DynamicMachineCodeCorrectTest : public DynamicMachineCodeTest {
public:
    explicit DynamicMachineCodeCorrectTest(const std::string& exepath, const std::string& testpath) : DynamicMachineCodeTest(std::cerr, std::cerr, std::cerr, stringstream, exepath, testpath) {}

    /**
     * Generic test body like present in many `TEST` and `TEST_F` cases.
     * This implementation checks whether the file pointed to by `path` can be converted to a syntaxtree without warnings, errors, segfaults.
     */
    void TestBody() override {
        ASSERT_EQ(exitTest(), 0);
        const std::string assemblypath = test::assemblypath_for(testpath);
        machinecode::generate(tree, table, logger, testpath, assemblypath, true);
        EXPECT_TRUE(test::has_success(logger));

        const auto executablepath = test::executablepath_for(assemblypath);
        if (int exitcode = test::compile(assemblypath, executablepath)) {
            std::cerr << "Received non-zero exitcode (" << exitcode << ") during compilation." << std::endl;
            GTEST_FAIL();
        }

        const double timeout = 3; // Each executable has this many seconds to finish. If it continues past this point, we assume the executable runs forever and we stop it.
        if (IOTest::has_io(testpath)) {
            auto testvector = IOTest::get_tests(testpath);
            for(const auto& testitem : testvector) {
                std::stringstream ss_in;
                const auto tokens_in = testitem.get_all(IOType::IN);
                std::for_each(std::begin(tokens_in), std::end(tokens_in), [&ss_in](const Token& token) -> void { ss_in << token.value << '\n'; });

                const auto cin = ss_in.str();
                subprocess::CompletedProcess completedprocess;
                ASSERT_NO_THROW(completedprocess = test::execute(executablepath, cin, timeout));
                EXPECT_EQ(completedprocess.returncode, 0);
                std::stringstream ss_out;
                const auto tokens_out = testitem.get_all(IOType::OUT);
                // TODO: remove after debug
//                if (this->testpath != "/home/mariska/uni/2021-2022/CoCo/liacs-coco/coco-reference/test/c-minus/programs/gcd/gcd.c")
//                    continue;
                std::for_each(std::begin(tokens_out), std::end(tokens_out), [&ss_out](const Token&  token) -> void { ss_out << token.value << '\n'; });
                const auto cout = ss_out.str();
                EXPECT_EQ(cout, completedprocess.cout);

            }
        } else {
            subprocess::CompletedProcess completedprocess;
            ASSERT_NO_THROW(completedprocess = test::execute(executablepath, "", timeout));
            EXPECT_EQ(completedprocess.returncode, 0);
        }

    }
protected:
    std::stringstream stringstream;
};
#endif