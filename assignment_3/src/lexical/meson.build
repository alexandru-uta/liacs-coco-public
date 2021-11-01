'''
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
'''

trigger_opt = get_option('with-lexical')
liblexical_depends = [libgeneral_dep, libsyntaxutils_dep, ghcfilesystem.get_variable('ghcfilesystem_dep')]
inc = include_directories('src/main/public')


if trigger_opt == 'own'
    liblexical_files = []
    subdir('src/main') # This adds all main source files

    liblexical = shared_library('lexical', liblexical_files, include_directories : inc, dependencies : liblexical_depends, install : true)
    liblexical_dep = declare_dependency(include_directories : inc, link_with : liblexical, dependencies : liblexical_depends)
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

    liblexical_files = []
    subdir('src/main')
    liblexical = shared_library('lexical', liblexical_files, include_directories : inc, dependencies : liblexical_depends, install : true, cpp_args : extra_args)
elif trigger_opt == 'prebuilt'
    compiler = meson.get_compiler('cpp')
    liblexical_dep  = declare_dependency(
        dependencies : [liblexical_depends, compiler.find_library('lexical', dirs : prebuiltdir)],
        include_directories : inc)
endif

if trigger_opt != 'make-prebuilt' and trigger_opt != 'make-prebuilt-debug'
    liblexical_exe = executable(
        'coco_compiler_lexical',
        'src/main/cpp/entrypoint/main.cpp',
        dependencies: liblexical_depends + [tclap.get_variable('tclap_dep'), liblexical_dep],
        build_rpath : prebuiltdir,
        install_rpath : prebuiltdir,
        include_directories : inc,
        install : true)
endif