#ifndef SRC_CLIENT_FIFO_H_
#define SRC_CLIENT_FIFO_H_

#include "../utils.h"

/**
 * @brief Checks if a public FIFO is open.
 * 
 * @param fifo_name the public FIFO name
 * @return int 0 upon success, -1 otherwise
 */
int isPublicFifoOpen(const char *fifo_name);

/**
 * @brief Gets the name of a private FIFO.
 * 
 * @param fifo_name the private FIFO name
 */
void getPrivateFifoName(char *fifo_name);

/**
 * @brief Sends a request to the server using the public FIFO already open.
 * 
 * @param np the file descriptor of the public FIFO
 * @param msg the message to send to the server
 */
void makeRequest(const int np, struct message msg);

/**
 * @brief Waits for the server response and receives the message by a private FIFO created for this task.
 * 
 * @param fifo_name the private FIFO name
 * @param msg the message to receive from the server
 */
void waitAnswer(const char *fifo_name, struct message *msg);

#endif // SRC_CLIENT_FIFO_H_
