trigger_opt = get_option('with-machine-code')
libmachinecode_depends = [libgeneral_dep, libsyntaxutils_dep, libsyntax_dep, libintermediatecode_dep, ghcfilesystem.get_variable('ghcfilesystem_dep')]
inc = include_directories('src/main/public')


# Sets RUNPATH/RPATH on compiled binaries to search for libraries on load-time.
# Priority list for finding libraries:
# 1. . (the current directory for our binary, should be equal to /prebuilt)
# 2. /build/src/lexical (where our 'own' libraries reside)
build_rpath = prebuiltdir_rpath_prebuilt+':'+join_paths(builddir_rpath_prebuilt, 'src', 'lexical')+':'+join_paths(builddir_rpath_prebuilt, 'src', 'syntaxutils')+':'+join_paths(builddir_rpath_prebuilt, 'src', 'syntax')+':'+join_paths(builddir_rpath_prebuilt, 'src', 'intermediate-code')
install_rpath = build_rpath



if trigger_opt == 'own'
    libmachinecode_files = []
    subdir('src/main') # This adds all main source files

    libmachinecode = library('machinecode', libmachinecode_files, include_directories : inc, dependencies: libmachinecode_depends, install : true, build_rpath : build_rpath, install_rpath : install_rpath)
    libmachinecode_dep = declare_dependency(include_directories : inc, link_with : libmachinecode, dependencies: libmachinecode_depends)
elif trigger_opt == 'make-prebuilt'  or trigger_opt == 'make-prebuilt-debug'
    if trigger_opt == 'make-prebuilt'
        compiler = meson.get_compiler('cpp')
        if compiler.get_id() == 'clang'
            message('Compiler: LLVM/clang')
            extra_args = ['-Ofast', '-funwind-tables', '-funroll-loops', '-mllvm', '-sub', '-mllvm', '-fla'] # hangs due to bcf (just slow or running forever?): ['-mllvm', '-bcf', '-mllvm', '-bcf_prob=1']
        elif compiler.get_id() == 'gcc'
            error('Can only use LLVM for obfuscated compilation')
        endif
    else
        extra_args = []
    endif

    libmachinecode_files = []
    subdir('src/main')
    libmachinecode = library('machinecode', libmachinecode_files, include_directories : inc, dependencies: libmachinecode_depends, install : true, build_rpath : build_rpath, install_rpath : install_rpath, cpp_args : extra_args)
elif trigger_opt == 'prebuilt'
    compiler = meson.get_compiler('cpp')
    libmachinecode_dep = declare_dependency(include_directories : inc, dependencies: libmachinecode_depends+[compiler.find_library('machinecode', dirs : prebuiltdir)])
endif


if trigger_opt != 'make-prebuilt' and trigger_opt != 'make-prebuilt-debug'
    libmachinecode_exe = executable(
        'coco_compiler_machinecode',
        'src/main/cpp/entrypoint/main.cpp',
        dependencies: libmachinecode_depends + [tclap.get_variable('tclap_dep'), libmachinecode_dep],
        build_rpath : build_rpath,
        install_rpath : install_rpath,
        install : true)
endif