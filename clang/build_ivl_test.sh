#!/usr/bin/env bash

set -euo pipefail
set -x

PROG_IDX="$1"
DIR="ivl_tests/$PROG_IDX"
[ -d "$DIR" ]
EXE="$DIR/exe"

SOURCES=("$DIR"/*cpp)

for SOURCE in "${SOURCES[@]}"
do
    /home/ilazaric/repos/ALL/submodules/objdir/llvm-project/bin/clang++ -O3 "$SOURCE" -c -o "$SOURCE.o"
done

/home/ilazaric/repos/ALL/submodules/objdir/llvm-project/bin/clang++ -O3 "${SOURCES[@]/%/.o}" -o "$EXE"

"./$EXE"
