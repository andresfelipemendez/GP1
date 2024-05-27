#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>

#define BATCH_SCRIPT L"dllbuild.bat"

void compile_dll() {
    // Call the batch script to compile the DLL
    if (system("dllbuild.bat") != 0) {
        printf("Compilation failed.\n");
    } else {
        printf("Compilation successful.\n");
    }
}

void watch_directory(const WCHAR* directory_to_watch) {
    HANDLE hDir = CreateFileW(
        directory_to_watch,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        wprintf(L"Failed to open directory for watching: %s (Error: %lu)\n", directory_to_watch, GetLastError());
        return;
    }

    char buffer[1024];
    DWORD bytesReturned;

    while (TRUE) {
        if (ReadDirectoryChangesW(
                hDir,
                &buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE,
                &bytesReturned,
                NULL,
                NULL
            )) {
            FILE_NOTIFY_INFORMATION *info = (FILE_NOTIFY_INFORMATION*)buffer;
            do {
                // Get the file name from the notification
                WCHAR fileName[MAX_PATH];
                wcsncpy(fileName, info->FileName, info->FileNameLength / sizeof(WCHAR));
                fileName[info->FileNameLength / sizeof(WCHAR)] = L'\0';

                // Check the file extension
                WCHAR *extension = wcsrchr(fileName, L'.');
                if (extension && (wcscmp(extension, L".c") == 0 || wcscmp(extension, L".cpp") == 0 || wcscmp(extension, L".h") == 0)) {
                    compile_dll();
                    break; // Compile once per change, you can remove this break to handle all changes
                }

                info = (FILE_NOTIFY_INFORMATION*)((char*)info + info->NextEntryOffset);
            } while (info->NextEntryOffset != 0);
        } else {
            printf("Failed to read directory changes.\n");
            break;
        }
    }

    CloseHandle(hDir);
}

int wmain(int argc, WCHAR *argv[]) {
    char* buffer;
    WCHAR directory_to_watch[MAX_PATH];

    if ((buffer = _getcwd(NULL, 0)) == NULL) {
        perror("_getcwd error");
        return 1;
    } else {
        mbstowcs(directory_to_watch, buffer, MAX_PATH);
        free(buffer);
    }

    wprintf(L"Watching directory: %s\n", directory_to_watch);
    watch_directory(directory_to_watch);
    return 0;
}
