#!/bin/bash

set -e

c="gcc"
cflags="-Wall -Wextra -O2 -Iinclude"
ldflags="-lncurses -lcurl"
target="build"

echo "Finding C source files..."
SRC=($(find . -name '*.c'))

OBJ=()
for f in "${SRC[@]}"; do
    f_clean="${f#./}"
    OBJ+=("${f_clean%.c}.o")
done

echo "Compiling source files..."
for i in "${!SRC[@]}"; do
    obj="${OBJ[$i]}"
    src="${SRC[$i]}"
    mkdir -p "$(dirname "$obj")"
    echo "  ✓ $src"
    $c $cflags -c "$src" -o "$obj"
done

echo "Linking object files..."
$c $cflags -o "$target" "${OBJ[@]}" $ldflags
echo "Build complete: $target"