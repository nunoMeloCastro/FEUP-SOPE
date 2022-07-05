#ifndef SRC_CLIENT_THREAD_H_
#define SRC_CLIENT_THREAD_H_

#include "../utils.h"

/**
 * @brief Initializes the semaphore that controls the access to shared variables between threads.
 */
void initSemaphore();

/**
 * @brief Increases the counting variable (without concurrence) 
 * 
 * @return an unique number
 */
int getThreadNumber();

/**
 * @brief Creates continuously request threads with random (pseudo-)intervals of milliseconds.
 * 
 * @param arg the arguments
 */
void *pthr_create(void *arg);

/**
 * @brief Sends requests to the server and waits for the response using the created threads.
 * 
 * @param arg the arguments
 */
void *pthr_request(void *arg);

#endif // SRC_CLIENT_THREAD_H_
