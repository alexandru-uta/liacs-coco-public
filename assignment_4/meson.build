project('coco-framework', 'cpp', version : '1.0', default_options : ['warning_level=3', 'c_std=c11', 'cpp_std=c++14'])

# We shut up gcc here about annoying gcc-only 'warnings'.
if meson.get_compiler('cpp').get_id() == 'gcc'
    add_global_arguments('-Wno-deprecated-copy', language: ['c', 'cpp'])
endif



build_system = build_machine.system() # Name of the system we build on.


gtest = subproject('gtest')
tclap = subproject('tclap-1.4.0-rc1_vendored')
ghcfilesystem = subproject('filesystem-1.5.4_vendored')
subprocess = subproject('subprocess_vendored')
mpark_variant = subproject('mpark_variant_vendored')
pico_sha2 = subproject('pico_sha2_vendored')


if build_system == 'windows'
  subproject('win-flex-bison-binaries') # Install flex/bison on Windows (hopefully).
endif


# Generators
flex = find_program('flex', required: true)
flex_gen = generator(
    flex,
    output: ['flex.@BASENAME@.cpp', 'flex.@BASENAME@.h'],
    arguments: [ '--outfile=@OUTPUT0@', '--header-file=@OUTPUT1@', '@INPUT@'])

bison = find_program('bison', required: true)
bison_gen = generator(
    bison,
    output: ['bison.@BASENAME@.cpp', 'bison.@BASENAME@.h'],
    arguments: [ '-Wall', '--output=@OUTPUT0@', '--defines=@OUTPUT1@', '@INPUT@' ])


# Variables
## Global variables
builddir_name = get_option('with-builddir')
prebuiltdir_name = get_option('with-prebuiltdir')
builddir_rpath_prebuilt = join_paths('$ORIGIN', '..', builddir_name)                           # RPATH to builddir when we are building a prebuilt library.
builddir_rpath_own = join_paths('$ORIGIN', '..', '..')                                         # RPATH to builddir when making our own binary.

prebuiltdir_rpath_prebuilt = '$ORIGIN'                                                         # RPATH to prebuiltdir when building a prebuilt library.
prebuiltdir_rpath_own = join_paths('$ORIGIN', '..', '..', '..', prebuiltdir_name)              # RPATH to prebuiltdir when making our own binary.


## Subproject variables
prebuiltdir = join_paths(meson.source_root(), prebuiltdir_name)
builddir = join_paths(meson.source_root(), builddir_name)

# binaries
subdir('src/general')
subdir('src/syntaxutils')

if get_option('with-tests')
    subdir('src/testutils')
endif

if get_option('with-lexical') != 'none'
    subdir('src/lexical')
endif
if get_option('with-syntax') != 'none'
    subdir('src/syntax')
endif
if get_option('with-intermediate-code') != 'none'
    subdir('src/intermediate-code')
endif
if get_option('with-machine-code') != 'none'
    subdir('src/machine-code')
endif

# test binaries
if get_option('with-tests')
    if get_option('with-lexical') == 'own' or get_option('with-lexical') == 'prebuilt'
        subdir('src/lexical/src/test')
    endif
    if get_option('with-syntax') == 'own' or get_option('with-syntax') == 'prebuilt'
        subdir('src/syntax/src/test')
    endif
    if get_option('with-machine-code') == 'own' or get_option('with-machine-code') == 'prebuilt'
        subdir('src/machine-code/src/test')
    endif
    if get_option('with-intermediate-code') == 'own' or get_option('with-intermediate-code') == 'prebuilt'
        if get_option('with-machine-code') == 'own' or get_option('with-machine-code') == 'prebuilt'
            subdir('src/intermediate-code/src/test')
        else
            warning('Skipping intermediatecode test executable, since no machinecode-library provided through either "with-machine-code=own" or "with-machine-code=prebuilt".')
        endif
    endif
endif


# Final notes
if get_option('with-lexical') == 'make-prebuilt-debug' or get_option('with-syntax') == 'make-prebuilt-debug' or get_option('with-intermediate-code') == 'make-prebuilt-debug' or get_option('with-machine-code') == 'make-prebuilt-debug'
    warning('One or more prebuilt-debug options have been set. Please be aware that you may not, under any circumstances, distribute resulting prebuilt-debug libraries to students, since: Students would be able to reverse-engineer these libraries quite easily to get the original source code back.')
endif