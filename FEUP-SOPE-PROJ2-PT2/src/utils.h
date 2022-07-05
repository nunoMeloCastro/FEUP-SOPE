#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <time.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>

#include "macros.h"


/**
 * @brief Struct for communication between client and server.
 * 
 */
struct message {
	int rid; // request id
	pid_t pid; // process id
	pthread_t tid; // thread id
	int tskload; // task load
	int tskres;	// task result
};

void printMessage(struct message msg);

/**
 * @brief Struct with the arguments passed to the server.
 * 
 */
typedef struct server_args {
    int nsecs;	// an amout of time in seconds
    char fifo_name[MAX_STR_LEN];	// the public FIFO name
} server_args;


/**
 * @brief Struct with the arguments passed to the producer thread.
 * 
 */
typedef struct producer_args {
    struct message *msg;	// an amout of time in seconds
    bool attend_request;	// the public FIFO file descriptor
} producer_args;


/**
 * @brief Checks if a given string represents a number.
 * 
 * @param str the string to check
 * @return 1 if the string is a number, 0 otherwise
 */
int isNumber(const char *str);

/**
 * @brief Gets the number of seconds elapsed since a determinated start time.
 * 
 * @param start the start time
 * @return time_t the elapsed time since the start time
 */
time_t getElapsedTimeSecs(time_t start);

/**
 * @brief Sleeps for a certain number of milliseconds.
 * 
 * @param num the number of milliseconds
 * @return int 0 on success, otherwise -1
 */
int sleepMillisecs(int num);

int compareMessages(const struct message* msg1, const struct message* msg2);

/**
 * @brief Registers operations in stdout in form of: inst ; i ; t ; pid ; tid ; res ; oper.
 * 
 * @param msg the struct with the information to be logged
 * @param oper the operation to be logged
 */
void writeLog(const struct message *msg, const char* oper);

#endif // SRC_UTILS_H_
