#!/bin/bash

# Define source and target variables
SOURCE="launcher/main.cpp"
TARGET="build/launcher"

# Ensure the build directory exists
mkdir -p build

# Compile the source file
g++ $SOURCE -o $TARGET -std=c++17

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable is located at $TARGET"
else
    echo "Compilation failed."
    exit 1
fi
