#include "hot_reload.h"
#include <iostream>

static HINSTANCE hLib = nullptr;

bool loadLibrary(const std::string& libName) {
    hLib = LoadLibraryA(libName.c_str());
    if (!hLib) {
        std::cerr << "Library couldn't be loaded: " << libName << " (Error: " << GetLastError() << ")\n";
        return false;
    }
    return true;
}

void unloadLibrary() {
    if (hLib) {
        FreeLibrary(hLib);
        hLib = nullptr;
    }
}

InitFunc getInitFunction() {
    return (InitFunc)GetProcAddress(hLib, "init");
}

UpdateFunc getUpdateFunction() {
    return (UpdateFunc)GetProcAddress(hLib, "update");
}

ShutdownFunc getShutdownFunction() {
    return (ShutdownFunc)GetProcAddress(hLib, "shutdown");
}

std::string getCurrentWorkingDirectory() {
    char* buffer;
    if ((buffer = getcwd(NULL, 0)) == NULL) {
        perror("_getcwd error");
        return "";
    } else {
        std::string cwd(buffer);
        free(buffer);
        return cwd;
    }
}
