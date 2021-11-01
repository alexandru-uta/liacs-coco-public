'''
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
'''

trigger_opt = get_option('with-syntax')
libsyntax_depends = [libgeneral_dep, libsyntaxutils_dep, ghcfilesystem.get_variable('ghcfilesystem_dep'), liblexical_dep]
inc = include_directories('src/main/public')


# Sets RUNPATH/RPATH on compiled binaries to search for libraries on load-time.
# Priority list for finding libraries:
# 1. . (the current directory for our binary, should be equal to /prebuilt)
# 2. /build/src/lexical (where our 'own' libraries reside)
build_rpath = prebuiltdir_rpath_prebuilt+':'+join_paths(builddir_rpath_prebuilt, 'src', 'lexical') +':'+join_paths(builddir_rpath_prebuilt, 'src', 'syntaxutils')
install_rpath = build_rpath



if trigger_opt == 'own'
    libsyntax_files = []
    subdir('src/main') # This adds all main source files

    libsyntax = library('syntax', libsyntax_files, include_directories : inc, dependencies : libsyntax_depends, install : true, build_rpath : build_rpath, install_rpath : install_rpath)
    libsyntax_dep = declare_dependency(include_directories : inc, link_with : libsyntax, dependencies: libsyntax_depends)
elif trigger_opt == 'make-prebuilt'  or trigger_opt == 'make-prebuilt-debug'
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

    libsyntax_files = []
    subdir('src/main')
    libsyntax = library('syntax', libsyntax_files, include_directories : inc, dependencies : libsyntax_depends, install : true, build_rpath : build_rpath, install_rpath : install_rpath, cpp_args : extra_args)
elif trigger_opt == 'prebuilt'
    compiler = meson.get_compiler('cpp')
    libsyntax_dep = declare_dependency(include_directories : inc, dependencies: libsyntax_depends+[compiler.find_library('syntax', dirs : prebuiltdir)])
endif


if trigger_opt != 'make-prebuilt' and trigger_opt != 'make-prebuilt-debug'
    libsyntax_exe = executable(
        'coco_compiler_syntax',
        'src/main/cpp/entrypoint/main.cpp',
        dependencies: libsyntax_depends + [tclap.get_variable('tclap_dep'), libsyntax_dep],
        include_directories : inc,
        build_rpath : build_rpath,
        install_rpath : install_rpath,
        install : true)
endif