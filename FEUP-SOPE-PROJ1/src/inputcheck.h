#ifndef INPUTCHECK_H_
#define INPUTCHECK_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "xmod.h"

#define INPUT_ERROR 2

/**
 * @brief Prints the Usage Wrong Message.
 * 
 */
void usageNotRight();

/**
 * @brief Checks if the input provided on the command line is Valid.
 * 
 * @param argc The number of arguments written in the command line
 * @param argv The array containing all the arguments passed on the execution of the program
 * @return 0 when there is an InputError, 1 otherwise
 */
int isValidInput(int argc, char *argv[]);

#endif /* INPUTCHECK_H_ */
