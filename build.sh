#!/bin/bash

set -e

CC="gcc"
CFLAGS="-Wall -Wextra -O2 -Iinclude"
LDFLAGS="-lncurses"
TARGET="build"

SRC=($(find . -name '*.c'))

OBJ=()
for f in "${SRC[@]}"; do
    f_clean="${f#./}"
    OBJ+=("${f_clean%.c}.o")
done

for i in "${!SRC[@]}"; do
    SRC_FILE="${SRC[$i]}"
    OBJ_FILE="${OBJ[$i]}"
    mkdir -p "$(dirname "$OBJ_FILE")"
    echo "Compiling $SRC_FILE -> $OBJ_FILE"
    $CC $CFLAGS -c "$SRC_FILE" -o "$OBJ_FILE"
done

$CC $CFLAGS -o "$TARGET" "${OBJ[@]}" $LDFLAGS

echo "Build complete: $TARGET"