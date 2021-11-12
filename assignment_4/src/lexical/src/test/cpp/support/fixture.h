/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_TEST_LEXICAL_FIXTURE
#define COCO_FRAMEWORK_TEST_LEXICAL_FIXTURE

#include "lexutil.h"

#include "gtest/gtest.h"
#include <test.h>

#include <logger.h>
#include <nothingvisitor.h>
#include <symboltable.h>
#include <syntaxtree.h>

#include <lexical.h>
#include <ostream>

class LexicalTest : public CapturingLoggingTest {
    public:
    LexicalTest() : CapturingLoggingTest(){};
    ~LexicalTest() override = default;

    /**
     * Simple function to test whether system is segfault-free. If we encounter a non-zero exit code, a segfault occurred.
     * @return Process exit code.
     */
    static int exitTest(const std::string& exepath, const std::string& testpath, bool no_warn=true, bool no_error=true) {
        std::stringstream ss;
        ss << exepath << " --no-print -f \"" << testpath << "\"";
        if (no_warn)
            ss << " --no-warn";
        if (no_error)
            ss << " --no-error";
        int exitCode = std::system(ss.str().c_str());
        return exitCode;
    }

    NothingVisitor vis{logger};
};

class DynamicLexicalTest : public LexicalTest {
    public:
    DynamicLexicalTest(const std::string& exepath, const std::string& testpath) : LexicalTest(), exepath(exepath), testpath(testpath){};
    ~DynamicLexicalTest() override = default;

    const std::string& exepath; // Path to the regular syntax executable.
    const std::string& testpath; // Path to the file we are currently testing.
};

class DynamicLexicalCorrectTest : public DynamicLexicalTest {
    public:
    explicit DynamicLexicalCorrectTest(const std::string& exepath, const std::string& testpath) : DynamicLexicalTest(exepath, testpath) {}
    ~DynamicLexicalCorrectTest() override = default;

    /**
     * Generic test body like present in many `TEST` and `TEST_F` cases.
     * This implementation checks whether the file pointed to by `path` can be tokenized by lexical rules without warnings, errors, segfaults.
     */
    void TestBody() override {
        ASSERT_EQ(exitTest(exepath, testpath), 0);
        int parseResult = lexical::generate(testpath, vis);
        EXPECT_TRUE(test::has_success(logger, parseResult, buf));
    }
};


class DynamicLexicalWarnTest : public DynamicLexicalTest {
public:
    explicit DynamicLexicalWarnTest(const std::string& exepath, const std::string& testpath) : DynamicLexicalTest(exepath, testpath) {}
    ~DynamicLexicalWarnTest() override = default;

    /**
     * Generic test body like present in many `TEST` and `TEST_F` cases.
     * This implementation checks whether the file pointed to by `path` succesfully fails to be tokenized by lexical rules.
     */
    void TestBody() override {
        ASSERT_EQ(exitTest(exepath, testpath), 0);
        int parseResult = lexical::generate(testpath, vis);
        EXPECT_TRUE(test::has_errors(logger, parseResult, buf));
    }
};

class DynamicLexicalErrorTest : public DynamicLexicalTest {
    public:
    explicit DynamicLexicalErrorTest(const std::string& exepath, const std::string& testpath) : DynamicLexicalTest(exepath, testpath) {}
    ~DynamicLexicalErrorTest() override = default;

    /**
     * Generic test body like present in many `TEST` and `TEST_F` cases.
     * This implementation checks whether the file pointed to by `path` succesfully fails to be tokenized by lexical rules.
     */
    void TestBody() override {
        ASSERT_EQ(exitTest(exepath, testpath), 0);
        int parseResult = lexical::generate(testpath, vis);
        EXPECT_TRUE(test::has_errors(logger, parseResult, buf));
    }
};
#endif