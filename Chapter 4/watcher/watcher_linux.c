#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>

#define BATCH_SCRIPT "./dllbuild.sh"
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

void compile_dll() {
    // Call the shell script to compile the DLL
    if (system(BATCH_SCRIPT) != 0) {
        printf("Compilation failed.\n");
    } else {
        printf("Compilation successful.\n");
    }
}

void watch_directory(const char* directory_to_watch) {
    int fd, wd;
    char buffer[EVENT_BUF_LEN];
    
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }
    
    wd = inotify_add_watch(fd, directory_to_watch, IN_MODIFY | IN_CREATE | IN_DELETE);
    if (wd == -1) {
        printf("Cannot watch directory '%s'\n", directory_to_watch);
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        int length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
        }

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                if (event->mask & IN_MODIFY) {
                    if (strstr(event->name, ".c") || strstr(event->name, ".cpp") || strstr(event->name, ".h")) {
                        compile_dll();
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

int main(int argc, char* argv[]) {
    char cwd[PATH_MAX];

    if (argc > 1) {
        if (chdir(argv[1]) != 0) {
            perror("chdir() to specified directory failed");
            return 1;
        }
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Watching directory: %s\n", cwd);
        watch_directory(cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    return 0;
}
