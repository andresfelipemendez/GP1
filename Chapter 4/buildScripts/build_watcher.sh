#!/bin/bash
mkdir -p build
gcc -o build/watcher_linux watcher/watcher_linux.c
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable is located at build/watcher_linux"
else
    echo "Compilation failed."
fi