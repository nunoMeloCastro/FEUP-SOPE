#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include "xmod.h"
#include "logger.h"
#include "signals.h"

// Macro: Identify the content's type
#define TYPE_OTHER 0
#define TYPE_DIR 1
#define TYPE_LNK 2

/**
 * Check the type of the content of a specific path 
 * @param path The location of the content to check
 * @return Number that identifies the type of the content the path represents
 */
int pathType(const char *path);

/**
 * Iterate a directory and change his content permissions, including himself
 * @param options Comand options
 * @param mode Comand new mode
 * @param dirpath The location of the directory to iterate
 * @return 0 when successfull, -1 otherwise
 */
int iterateDirectory(const char *options, const char *mode, const char *dirpath);

#endif /* FILESYSTEM_H_ */
