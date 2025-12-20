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
OBJECT="$BASENAME.o"
EXECUTABLE="$BASENAME"

echo "Assembling $SOURCE..."
yasm -f elf64 "$SOURCE" -o "$OBJECT"

echo "Linking $OBJECT..."
ld "$OBJECT" -o "$EXECUTABLE"

echo "Build successful!"
echo "Executable: ./$EXECUTABLE"

