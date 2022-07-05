#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "xmod.h"
#include "filesystem.h"
#include "inputcheck.h"
#include "logger.h"
#include "signals.h"

#define MAX_STR_LEN 256

//char info[MAX_STR_LEN];
extern char *process_path;
extern char line_args[MAX_STR_LEN];
extern int env_def;

int main(int argc, char *argv[], char *envp[]) {
    setUpSignals();

    // Check program call
    if (!isValidInput(argc, argv)) {
        usageNotRight();
        exit(INPUT_ERROR);
    }

    // With the valid input we can save the values without any problem
    char *mode = argv[argc - 2];
    char *path_name = argv[argc - 1];
    char *options = argc == 3 ? NULL : argv[1];
    int opt_R = options != NULL && strchr(options, 'R') != NULL ? 1 : 0;

    process_path = path_name;

    getLineArgs(mode, path_name, options);
    env_def = openLogger();
    writeToLogger(getpid(), PROC_CREAT, line_args);

    if ((pathType(path_name) == TYPE_DIR) && opt_R) {
        // directory to iterate
        if (iterateDirectory(options, mode, path_name) != 0) {
            fprintf(stderr, "Error changing dir's files permissions\n");
            exit(EXIT_FAILURE);
        }
    } else {
        // single file/directory/symbolic link
        if (xmod(options, mode, path_name) != 0) {
            fprintf(stderr, "Error changing file/directory's permissions\n");
            exit(EXIT_FAILURE);
        }
    }

    closeLogger();

    return 0;
}
