#!/bin/bash
# Very basic script to execute clang-format on all sourcefiles, particularly excluding c-minus tests.
folder=.
exclude_folder=*c-minus*
format_files=`find "${folder}" -type f ! -path "${exclude_folder}" -iname "*.h" -o -iname "*.cpp" -prune`

for file in $format_files
do
  clang-format -i "$file"
done