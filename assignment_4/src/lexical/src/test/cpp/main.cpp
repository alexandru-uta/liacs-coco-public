/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include "support/fixture.h"
#include "gtest/gtest.h"
#include <dynamic_test.h>
#include <ghc/filesystem.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    ghc::filesystem::path root_path = ghc::filesystem::canonical(ghc::filesystem::absolute(argv[0])); // Distro-dependent method to get executable-path.
    std::cerr << "Test root path is: " << root_path << std::endl;

    ghc::filesystem::path project_root_path = root_path.parent_path().parent_path().parent_path().parent_path().parent_path().parent_path(); // Project root path.

    ghc::filesystem::path exe_path = root_path.parent_path().parent_path().parent_path() / "coco_compiler_lexical"; // Absolute path to regular executable.
    if (!ghc::filesystem::exists(exe_path))
        throw std::runtime_error("Path does not exist: "+exe_path.string());

    ghc::filesystem::path test_path_general = project_root_path / "test" / "c-minus"; // Absolute path to general tests.
    if (!ghc::filesystem::exists(test_path_general))
        throw std::runtime_error("Path does not exist: "+test_path_general.string());

    ghc::filesystem::path test_path_specific = project_root_path / "src" / "lexical" / "src" / "test" / "c-minus"; // Absolute path to lexical tests.
    if (!ghc::filesystem::exists(test_path_specific))
        throw std::runtime_error("Path does not exist: "+test_path_specific.string());


    ////////////////////// Execute global tests //////////////////////
    test::support::register_tests<LexicalTest>(project_root_path, exe_path, test_path_general, "DynamicLexicalCorrectTest", {"incorrect", "warn"}, [](const std::string& exe_path, const std::string& test_path) -> LexicalTest* {
        return new DynamicLexicalCorrectTest(exe_path, test_path);
    });

    ////////////////////// Execute lexical-specific tests //////////////////////
    test::support::register_tests<LexicalTest>(project_root_path, exe_path, test_path_specific, "DynamicLexicalCorrectTest", {"incorrect", "warn"},[](const std::string& exe_path, const std::string& test_path) -> LexicalTest* {
        return new DynamicLexicalCorrectTest(exe_path, test_path);
    });
    test::support::register_tests<LexicalTest>(project_root_path, exe_path, test_path_specific, "DynamicLexicalCorrectTest", {"correct", "incorrect"},[](const std::string& exe_path, const std::string& test_path) -> LexicalTest* {
        return new DynamicLexicalWarnTest(exe_path, test_path);
    });

    test::support::register_tests<LexicalTest>(project_root_path, exe_path, test_path_specific, "DynamicLexicalErrorTest", {"correct", "warn"},[](const std::string& exe_path, const std::string& test_path) -> LexicalTest* {
        return new DynamicLexicalErrorTest(exe_path, test_path);
    });
    return RUN_ALL_TESTS();
}