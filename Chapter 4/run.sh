#!/bin/bash

# Function to check if inotify-tools is installed
check_inotify_tools() {
    if ! command -v inotifywait &> /dev/null; then
        echo "inotify-tools is not installed."
        read -p "Do you want to install inotify-tools? (y/n) " choice
        case "$choice" in 
            y|Y ) 
                echo "Installing inotify-tools..."
                sudo apt-get update
                sudo apt-get install -y inotify-tools
                ;;
            * ) 
                echo "inotify-tools is required to run this script. Exiting."
                exit 1
                ;;
        esac
    else
        echo "inotify-tools is already installed."
    fi
}

# Check and install inotify-tools if necessary
check_inotify_tools

# Call the build_watcher.sh script
./buildScripts/build_watcher.sh

# Check if build_watcher.sh was successful
if [ $? -ne 0 ]; then
    echo "build_watcher.sh failed. Exiting."
    exit 1
fi

# Call the build_launcher.sh script
./buildScripts/build_launcher.sh

# Check if build_launcher.sh was successful
if [ $? -ne 0 ]; then
    echo "build_launcher.sh failed. Exiting."
    exit 1
fi

# Run the launcher executable
./build/watcher_linux
