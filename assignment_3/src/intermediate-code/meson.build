trigger_opt = get_option('with-intermediate-code')
inc = include_directories('src/main/public')
libintermediatecode_depends = [libgeneral_dep, liblexical_dep, libsyntaxutils_dep, libsyntax_dep, ghcfilesystem.get_variable('ghcfilesystem_dep')]


# Sets RUNPATH/RPATH on compiled binaries to search for libraries on load-time.
# Priority list for finding libraries:
# 1. . (the current directory for our binary, should be equal to /prebuilt)
# 2. /build/src/lexical (where our 'own' libraries reside)
build_rpath = prebuiltdir_rpath_prebuilt+':'+join_paths(builddir_rpath_prebuilt, 'src', 'lexical')+':'+join_paths(builddir_rpath_prebuilt, 'src', 'syntaxutils')+':'+join_paths(builddir_rpath_prebuilt, 'src', 'syntax')
install_rpath = build_rpath

if trigger_opt == 'own'
    libintermediatecode_files = []
    subdir('src/main') # This adds all main source files

    libintermediatecode = library('intermediatecode', libintermediatecode_files, include_directories : inc, dependencies: libintermediatecode_depends, install : true, build_rpath : build_rpath, install_rpath : install_rpath)
    libintermediatecode_dep = declare_dependency(include_directories : inc, link_with : libintermediatecode, dependencies: libintermediatecode_depends)
elif trigger_opt == 'make-prebuilt' or trigger_opt == 'make-prebuilt-debug'
    if trigger_opt == 'make-prebuilt'
        compiler = meson.get_compiler('cpp')
        if compiler.get_id() == 'clang'
            message('Compiler: LLVM/clang')
            extra_args = ['-Ofast', '-funwind-tables', '-funroll-loops', '-mllvm', '-sub', '-mllvm', '-fla']
        elif compiler.get_id() == 'gcc'
            error('Can only use LLVM for obfuscated compilation')
        endif
    else
        extra_args = []
    endif

    libintermediatecode_files = []
    subdir('src/main')
    libintermediatecode = library('intermediatecode', libintermediatecode_files, include_directories : inc, dependencies: libintermediatecode_depends, install : true, build_rpath : build_rpath, install_rpath : install_rpath, cpp_args : extra_args)
elif trigger_opt == 'prebuilt'
    compiler = meson.get_compiler('cpp')
    libintermediatecode_dep = declare_dependency(include_directories : inc, dependencies: libintermediatecode_depends+[compiler.find_library('intermediatecode', dirs : prebuiltdir)])
endif


if trigger_opt != 'make-prebuilt' and trigger_opt != 'make-prebuilt-debug'
    libintermediatecode_exe = executable(
        'coco_compiler_intermediatecode',
        'src/main/cpp/entrypoint/main.cpp',
        dependencies: libintermediatecode_depends + [tclap.get_variable('tclap_dep'), libintermediatecode_dep],
        build_rpath : build_rpath,
        install_rpath : install_rpath,
        install : true)


    if get_option('with-tests')
        # Pass. We require to have the machinecode library evaluated and ready for use.
        # We handle intermediatecode test building after the machinecode evaluation pass has completed.
    endif
endif