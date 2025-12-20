#!/usr/bin/env bash

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 <file.asm>"
    exit 1
fi

SOURCE="$1"

if [ ! -f "$SOURCE" ]; then
    echo "Error: File '$SOURCE' not found"
    exit 1
fi

BASENAME="${SOURCE%.asm}"
OBJECT="bin/$BASENAME.o"
EXECUTABLE="bin/$BASENAME"
LISTFILE="bin/$BASENAME.lst"
echo "Assembling $SOURCE..."
yasm -f elf64 -g dwarf2 "$SOURCE" -o "$OBJECT"
yasm -f elf64 -g dwarf2 "$SOURCE" -l "$LISTFILE"
echo "Linking $OBJECT..."
ld -g "$OBJECT" -o "$EXECUTABLE"

echo "Build successful!"
echo "Executable: ./$EXECUTABLE"

