#include "support/fixture.h"
#include "gtest/gtest.h"
#include <dynamic_test.h>
#include <ghc/filesystem.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    ghc::filesystem::path root_path = ghc::filesystem::canonical(ghc::filesystem::absolute(argv[0])); // Distro-dependent method to get executable-path.
    std::cerr << "Test root path is: " << root_path << std::endl;

    ghc::filesystem::path project_root_path = root_path.parent_path().parent_path().parent_path().parent_path().parent_path().parent_path(); // Project root path.

    ghc::filesystem::path exe_path = root_path.parent_path().parent_path().parent_path() / "coco_compiler_machinecode"; // Absolute path to regular executable.
    if (!ghc::filesystem::exists(exe_path))
        throw std::runtime_error("Path does not exist: "+exe_path.string());

    ghc::filesystem::path test_path_general = project_root_path / "test" / "c-minus"; // Absolute path to general tests.
    if (!ghc::filesystem::exists(test_path_general))
        throw std::runtime_error("Path does not exist: "+test_path_general.string());

    ////////////////////// Execute global tests //////////////////////
    test::support::register_tests<MachineCodeTest>(project_root_path, exe_path, test_path_general, "DynamicMachineCodeCorrectTest", {"incorrect", "warn"}, [](const std::string& exe_path, const std::string& test_path) -> MachineCodeTest* {
        return new DynamicMachineCodeCorrectTest(exe_path, test_path);
    });
    return RUN_ALL_TESTS();
}