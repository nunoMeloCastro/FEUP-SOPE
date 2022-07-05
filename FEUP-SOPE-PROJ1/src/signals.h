#ifndef SIGNALS_H_
#define SIGNALS_H_

#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#include "utils.h"
#include "logger.h"

#define STR_SIGINT "SIGINT"
#define STR_SIGUSR1 "SIGUSR1"
#define STR_SIGUSR2 "SIGUSR2"
#define STR_SIGTERM "SIGTERM"
#define STR_SIGSTOP "SIGSTOP"
#define STR_SIGCONT "SIGCONT"

#define EXIT_CTRLC 5

void setUpSignals();

char *getSignalName(int sig);

/**
 * @brief Register the reception of signals to be ignored
 * 
 * @param sig received signal
 */
void registerAndIgnore(int sig);

/**
 * @brief Handle interrupt - order the children to write their information
 * 
 * @param sig received signal
 */
void ctrlcReceived(int sig);

/**
 * @brief Write the process information on the screen and stop him self
 * 
 * @param sig received signal
 */
void displayInfo(int sig);

/**
 * @brief Confirm the termination of the program and operate according the answer
 * 
 * @param sig received signal
 */
void questionPrompt(int sig);

/**
 * @brief Register the termination of a process and exit it
 * 
 * @param sig received signal
 */
void terminate(int sig);

#endif /* SIGNALS_H_ */
