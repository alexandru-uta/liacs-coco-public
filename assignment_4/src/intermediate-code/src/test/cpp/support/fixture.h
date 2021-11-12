#ifndef COCO_FRAMEWORK_TEST_INTERMEDIATECODE_FIXTURE
#define COCO_FRAMEWORK_TEST_INTERMEDIATECODE_FIXTURE

#include "intermediatecodeutil.h"
#include "gtest/gtest.h"

#include <test.h>


#include <syntax.h>
#include <logger.h>
#include <symboltable.h>
#include <syntaxtree.h>
#include <machinecode.h>
#include <codegenerator.h>

#include <ostream>
#include <intermediatecode.h>
#include <ghc/filesystem.h>


class IntermediatecodeTest : public CapturingLoggingTest {
protected:
    IntermediatecodeTest(std::ostream& infostream, std::ostream& warnstream, std::ostream& errorstream, std::ostream& out) : logger(infostream, warnstream, errorstream), out(out) {}

    Logger logger;
    SymbolTable table{};
    SyntaxTree tree{};
    std::ostream& out;
};

class IntermediateCorrectTest : public IntermediatecodeTest {
protected:
    IntermediateCorrectTest() : IntermediatecodeTest(std::cerr, std::cerr, std::cerr, std::cerr) {}
};

class DynamicIntermediatecodeTest : public IntermediatecodeTest, public IOTest {
protected:
    explicit DynamicIntermediatecodeTest(std::ostream& infostream, std::ostream& warnstream, std::ostream& errorstream, std::ostream& out, const std::string& exepath, const std::string& testpath) : IntermediatecodeTest(infostream, warnstream, errorstream, out), IOTest(), exepath(exepath), testpath(testpath){};

    /**
     * Simple function to test whether our system is segfault-free.
     * If we execute this function from another thread and that thread segfaults, its exitcode is never 0.
     * By checking the thread exitcode, we know whether we segfaulted.
     * @param no_warn If set, we forward a flag that disables compiler-printed warnings. This is useful to not spam the console when doing warning-tests.
     */
    inline int exitTest(bool no_warn=false) const {
        std::stringstream ss;
        ss << exepath << " --no-print";
        if (no_warn)
            ss << " --no-warn";
        ss << " -f \"" << testpath << "\"";
        int exitCode = std::system(ss.str().c_str());
        return exitCode;
    }

    const std::string& exepath; // Path to the file we are currently testing.
    const std::string& testpath; // Path to the file we are currently testing.
};

class DynamicIntermediatecodeCorrectTest : public DynamicIntermediatecodeTest {
public:
    explicit DynamicIntermediatecodeCorrectTest(const std::string& exepath, const std::string& testpath) : DynamicIntermediatecodeTest(std::cerr, std::cerr, std::cerr, stringstream, exepath, testpath) {}

    /**
     * Generic test body like present in many `TEST` and `TEST_F` cases.
     * This implementation checks whether the file pointed to by `path` can be compiled without warnings, errors, segfaults.
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

class DynamicIntermediatecodeWarnTest : public DynamicIntermediatecodeTest {
public:
    explicit DynamicIntermediatecodeWarnTest(const std::string& exepath, const std::string& testpath) : DynamicIntermediatecodeTest(std::cerr, NULL_STREAM, std::cerr, stringstream, exepath, testpath) {}

    /**
     * Generic test body like present in many `TEST` and `TEST_F` cases.
     * This implementation checks whether the file pointed to by `path` can be compiled without warnings, errors, segfaults.
     */
    void TestBody() override {
        ASSERT_EQ(exitTest(true), 0);
        const std::string assemblypath = test::assemblypath_for(testpath);
        machinecode::generate(tree, table, logger, testpath, assemblypath, true);
        EXPECT_TRUE(test::has_warnings(logger));

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

class DynamicIntermediatecodeErrorTest : public DynamicIntermediatecodeTest {
public:
    explicit DynamicIntermediatecodeErrorTest(const std::string& exepath, const std::string& testpath) : DynamicIntermediatecodeTest(std::cerr, std::cerr, NULL_STREAM, stringstream, exepath, testpath) {}

    /**
     * Generic test body like present in many `TEST` and `TEST_F` cases.
     * This implementation checks whether the file pointed to by `path` successfully fails to compile.
     */
    void TestBody() override {
        ASSERT_EQ(exitTest(), 0);
        const std::string assemblypath = test::assemblypath_for(testpath);
        machinecode::generate(tree, table, logger, testpath, assemblypath, true);
        EXPECT_TRUE(test::has_errors(logger));
    }
protected:
    std::stringstream stringstream;
};

#endif