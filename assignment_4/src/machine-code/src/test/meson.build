gtest_dep = gtest.get_variable('gtest_dep')
subprocess_dep = subprocess.get_variable('subprocess_dep')
mpark_variant_dep = mpark_variant.get_variable('mpark_variant_dep')

libmachinecode_test_depends = [gtest_dep, mpark_variant_dep, subprocess_dep, libtestutils_dep]
libmachinecode_test_depends += libmachinecode_depends
libmachinecode_test_depends += libmachinecode_dep

libmachinecode_test_files = []
libmachinecode_test_files += files('cpp/main.cpp')

libmachinecode_test_exe = executable(
    'libmachinecode_test',
    libmachinecode_test_files,
    dependencies: libmachinecode_test_depends,
    include_directories : inc,
    build_rpath : build_rpath,
    install_rpath : install_rpath,
    install : true)