'''
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
'''

trigger_opt = get_option('with-syntax')
inc = include_directories('src/main/public')
libsyntaxutils_depends = [libgeneral_dep]


# Sets RUNPATH/RPATH on compiled binaries to search for libraries on load-time.
# Priority list for finding libraries:
# 1. . (the current directory for our binary, should be equal to /prebuilt)
build_rpath = prebuiltdir_rpath_prebuilt
install_rpath = build_rpath


if trigger_opt == 'own'    # own syntax --> own syntaxutils
    libsyntaxutils_files = []
    subdir('src/main') # This adds all main source files

    libsyntaxutils = library('syntaxutils', libsyntaxutils_files, include_directories : inc, dependencies : libsyntaxutils_depends, install : true)
    libsyntaxutils_dep = declare_dependency(include_directories : inc, link_with : libsyntaxutils, dependencies: libsyntaxutils_depends)
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

    libsyntaxutils_files = []
    subdir('src/main')
    libsyntaxutils = library('syntaxutils', libsyntaxutils_files, include_directories : inc, dependencies : libsyntaxutils_depends, install : true, build_rpath : build_rpath, install_rpath : install_rpath, cpp_args : extra_args)
    libsyntaxutils_dep = declare_dependency(include_directories : inc, link_with : libsyntaxutils, dependencies: libsyntaxutils_depends) # Added syntaxutils as an exception, since `lexical` requires it when building.
elif trigger_opt == 'prebuilt'
    compiler = meson.get_compiler('cpp')
    libsyntaxutils_dep = declare_dependency(include_directories : inc, dependencies: libsyntaxutils_depends+[compiler.find_library('syntaxutils', dirs : prebuiltdir)])
endif