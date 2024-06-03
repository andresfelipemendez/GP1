#!/bin/bash

# Define source and target variables
SOURCE="launcher/main.cpp"
TARGET="build/launcher"
LIB_PATH="../Lib"
# Ensure the build directory exists
mkdir -p build

# Compile the source file
g++ $SOURCE launcher/hot_reload_linux.cpp -o $TARGET -std=c++17 -I$LIB_PATH/entt/include -ldl

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable is located at $TARGET"
else
    echo "Compilation failed."
    exit 1
fi
