#ifndef SRC_CLIENT_INPUT_CHECK_H_
#define SRC_CLIENT_INPUT_CHECK_H_

/**
 * @brief Prints the program usage information.
 * 
 */
void usageNotRight();

/**
 * @brief Checks if the input provided on the command line is valid.
 * 
 * @param argc the number of arguments written in the command line
 * @param argv the array containing all the arguments passed on the execution of the program
 * @return 0 upon success, 1 otherwise
 */
int isValidInput(int argc, char *argv[]);

#endif // SRC_CLIENT_INPUT_CHECK_H_
