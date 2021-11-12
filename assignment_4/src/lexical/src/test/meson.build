gtest_dep = gtest.get_variable('gtest_dep')

liblexical_test_depends = [gtest_dep, libtestutils_dep]
liblexical_test_depends += liblexical_depends
liblexical_test_depends += liblexical_dep

liblexical_test_files = []
liblexical_test_files += files('cpp/main.cpp')

liblexical_test_exe = executable(
    'liblexical_test',
    liblexical_test_files,
    dependencies: liblexical_test_depends,
    build_rpath : prebuiltdir,
    install_rpath : prebuiltdir,
    install : true)
