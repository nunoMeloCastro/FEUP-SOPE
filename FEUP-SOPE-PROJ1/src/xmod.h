#ifndef XMOD_H_
#define XMOD_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "logger.h"

#define MAX_STR_LEN 256
#define RESET_MODE ~(000777)
#define GET_MODE 000777

int xmod(const char *options, const char *mode, const char *path_name);

int handleOptions(const char *options, const char *path_name, const mode_t new_perms);

char *getStrPerms(const char *mode);

mode_t getNewPerms(const char *mode, const char *path_name);

mode_t resetModeUser(const mode_t current_mode, const char user);

#endif /* XMOD_H_ */
