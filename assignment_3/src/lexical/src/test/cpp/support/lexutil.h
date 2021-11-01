#ifndef COCO_FRAMEWORK_TEST_LEXICAL_LEXUTIL
#define COCO_FRAMEWORK_TEST_LEXICAL_LEXUTIL

#include "gtest/gtest.h"
#include <logger.h>

namespace test {
    inline testing::AssertionResult has_errors(const Logger& logger, int lexcode, std::ostringstream& buf) {
        if (lexcode == 0)
            return testing::AssertionFailure() << "Encountered lexcode=0 for error test. Found output:\n"
            << buf.str();
        if (logger.n_warnings() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_warnings() << " warning(s). Found output:\n"
            << buf.str();
        if (logger.n_errors() == 0)
            return testing::AssertionFailure() << "Did not log any errors using the logger. Found output:\n"
            << buf.str();
        else
            return testing::AssertionSuccess();
    }

    inline testing::AssertionResult has_warnings(const Logger& logger, int lexcode, std::ostringstream& buf) {
        if (lexcode == 0)
            return testing::AssertionFailure() << "Encountered lexcode=0 for incorrect test. Found output:\n"
            << buf.str();
        if (logger.n_errors() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_errors() << " error(s). Found output:\n"
            << buf.str();
        if (logger.n_warnings() == 0)
            return testing::AssertionFailure() << "Did not log any warnings using the logger. Found output:\n"
            << buf.str();
        else
            return testing::AssertionSuccess();
    }


    inline testing::AssertionResult has_success(const Logger& logger, int lexcode, std::ostringstream& buf) {
        if (lexcode != 0)
            return testing::AssertionFailure() << "Encountered lexcode=" << lexcode << " for correct test. Found output:\n"
            << buf.str();
        if (logger.n_warnings() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_warnings() << " warning(s). Found output:\n"
            << buf.str();
        if (logger.n_errors() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_errors() << " error(s). Found output:\n"
            << buf.str();
        else
            return testing::AssertionSuccess();
    }
}

#endif
