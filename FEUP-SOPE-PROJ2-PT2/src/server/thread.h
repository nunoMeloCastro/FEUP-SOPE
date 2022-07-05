#ifndef SRC_SERVER_THREAD_H_
#define SRC_SERVER_THREAD_H_

#include "../utils.h"

/**
 * @brief Receives requests from the client and create producer threads for each one.
 * 
 * @param arg the arguments
 */
void *pthr_receiver(void *arg);


void *pthr_producer(void *arg);


void *pthr_consumer(void *arg);

#endif // SRC_SERVER_THREAD_H_