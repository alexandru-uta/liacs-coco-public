#include "dynamic_test.h"
#include <ghc/filesystem.h>

// https://stackoverflow.com/questions/19160244/create-tests-at-run-time-google-test
namespace test {
    namespace support {
        namespace internal {
            static std::string dirname(const ghc::filesystem::path& path) {
                auto path_str = ghc::filesystem::is_directory(path) ? path.generic_string() : path.parent_path().generic_string();
                size_t found = path_str.find_last_of("/\\");
                return path_str.substr(found + 1);
            }

            std::vector<std::string> test_iterator(const std::string& root, const std::unordered_set<std::string>& excluded_dirs) {
                std::vector<std::string> paths;
                auto iterator = ghc::filesystem::recursive_directory_iterator(root);
                for (decltype(iterator) end; iterator != end; ++iterator) {
                    auto p = iterator->path();
                    if (ghc::filesystem::is_directory(p)) {                        //exclude directories from output.
                        if (excluded_dirs.find(dirname(p)) != excluded_dirs.end()) // Skip directory if in exclusion dirnames.
                            iterator.disable_recursion_pending();
                        continue;
                    }
                    if (p.extension() != ".c") // We are not dealing with a c-minus file.
                        continue;
                    // We have a file
                    paths.emplace_back(p.generic_string());
                }
                return paths;
            }

            std::vector<std::string> test_iterator(const std::string& root) {
                std::vector<std::string> paths;
                auto iterator = ghc::filesystem::recursive_directory_iterator(root);
                for (decltype(iterator) end; iterator != end; ++iterator) {
                    auto p = iterator->path();
                    if (ghc::filesystem::is_directory(p)) //exclude directories from output
                        continue;
                    // We have a file
                    paths.emplace_back(p.generic_string());
                }
                return paths;
            }
        }
    }
}