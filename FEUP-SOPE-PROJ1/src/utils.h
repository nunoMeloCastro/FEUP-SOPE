#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX_STR_LEN 256
#define RESET_MODE_USR ~(000700)
#define RESET_MODE_GRP ~(000070)
#define RESET_MODE_OTH ~(000007)
#define RESET_MODE_ALL ~(000777)

char* getFileName(const char* path_name);

char* convertPermsToString(const mode_t perms);

/**
 * @brief Checks if a given string represents a number.
 * 
 * @param str The string to check
 * @return 1 if the string is a number, 0 otherwise
 */
int isNumber(const char *str);

/**
 * @brief Checks if a given string contains a scpecific character.
 * 
 * @param possible_str The string to check
 * @param n The length of the string
 * @param ch The char to search
 * @return 1 if the string constains the character, 0 otherwise
 */
int findChar(const char possible_str[], unsigned n, const char ch);

double getElapsedTime(const struct timespec start);

void getLineArgs(const char *mode, const char *path, const char *options);

/**
 * @brief Waits for a input of 'y' or 'n'.
 * 
 * @return The answer
 */
char readAnswer(void);

int counterChar(const char* str, const char ch);

void upperCase(char *s);

#endif /* UTILS_H_ */
