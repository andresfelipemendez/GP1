// hotreload.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <filesystem>
#include <chrono>
#include <string>
#include <direct.h>

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

namespace fs = std::filesystem;

typedef int (*FnEnginelib)();

int main()
{    
    char* buffer;
    WCHAR directory_to_watch[MAX_PATH];

    if ((buffer = _getcwd(NULL, 0)) == NULL) {
        perror("_getcwd error");
        return 1;
    } else {
        mbstowcs(directory_to_watch, buffer, MAX_PATH);
        free(buffer);
    }

    std::wstring ws_directory_to_watch(directory_to_watch);
    std::string directory_to_watch_str(ws_directory_to_watch.begin(), ws_directory_to_watch.end());

    // Constructing paths for DLLs
    std::string originalDLLpath = directory_to_watch_str + "\\game.dll";
    std::string copyDLLpath = directory_to_watch_str + "\\game_copy.dll";

    if (!fs::exists(originalDLLpath)) {
        std::cerr << "Original DLL not found: " << originalDLLpath << '\n';
        return -1;
    } else {
        fs::copy(originalDLLpath, copyDLLpath, fs::copy_options::overwrite_existing);
    }

    auto lastModifiedTime = std::filesystem::last_write_time(originalDLLpath);

    HINSTANCE engineLibrary = LoadLibraryA(copyDLLpath.c_str());
    if (!engineLibrary) {
        std::cerr << "Library couldn't be loaded: " << copyDLLpath << " (Error: " << GetLastError() << ")\n";
        return -1;
    }
    
    FnEnginelib enginefunction = (FnEnginelib)GetProcAddress(engineLibrary, "fnenginelib");

    if (!enginefunction) {
        std::cerr << "Unable to find fnenginelib" << std::endl;
        FreeLibrary(engineLibrary);
        return -1;
    }

    enginefunction();

    FreeLibrary(engineLibrary);

    return 0;
}
