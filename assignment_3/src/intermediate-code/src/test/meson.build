gtest_dep = gtest.get_variable('gtest_dep')

libintermediatecode_test_depends = [gtest_dep, libtestutils_dep]
libintermediatecode_test_depends += libintermediatecode_depends
libintermediatecode_test_depends += libintermediatecode_dep
libintermediatecode_test_depends += libmachinecode_dep
libintermediatecode_test_depends += libmachinecode_test_depends

libintermediatecode_test_files = []
libintermediatecode_test_files += files('cpp/main.cpp', 'cpp/units/api.cpp')


libintermediatecode_test_exe = executable(
    'libintermediatecode_test',
    libintermediatecode_test_files,
    dependencies: libintermediatecode_test_depends,
    build_rpath : build_rpath,
    install_rpath : install_rpath,
    install : true)
