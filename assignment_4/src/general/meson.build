inc = include_directories('src/main/public')

libgeneral_files = []
subdir('src/main') # This adds all main source files

libgeneral = library('general', libgeneral_files, include_directories : inc, install : true)
libgeneral_dep = declare_dependency(include_directories : inc, link_with : libgeneral)