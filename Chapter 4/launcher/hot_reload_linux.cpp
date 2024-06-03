#include "hot_reload.h"
#include <iostream>

static void* hLib = nullptr;

bool loadLibrary(const std::string& libName) {
    hLib = dlopen(libName.c_str(), RTLD_LAZY);
    if (!hLib) {
        std::cerr << "Library couldn't be loaded: " << libName << " (Error: " << dlerror() << ")\n";
        return false;
    }
    return true;
}

void unloadLibrary() {
    if (hLib) {
        dlclose(hLib);
        hLib = nullptr;
    }
}

InitFunc getInitFunction() {
    return (InitFunc)dlsym(hLib, "init");
}

UpdateFunc getUpdateFunction() {
    return (UpdateFunc)dlsym(hLib, "update");
}

ShutdownFunc getShutdownFunction() {
    return (ShutdownFunc)dlsym(hLib, "shutdown");
}

std::string getCurrentWorkingDirectory() {
    char* buffer;
    if ((buffer = getcwd(NULL, 0)) == NULL) {
        perror("getcwd error");
        return "";
    } else {
        std::string cwd(buffer);
        free(buffer);
        return cwd;
    }
}