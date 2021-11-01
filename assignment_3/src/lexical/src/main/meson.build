liblexical_files += files(
    'cpp/debug/debug.cpp',
    'cpp/visitor/nothing/nothingvisitor.cpp',
    'cpp/visitor/basevisitor.cpp',
    'cpp/lexical.cpp')
liblexical_files += flex_gen.process('flex_bison/compiler.l')
liblexical_files += bison_gen.process('flex_bison/compiler.y')