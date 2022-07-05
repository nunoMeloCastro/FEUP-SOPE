#ifndef SRC_SERVER_FIFO_H_
#define SRC_SERVER_FIFO_H_

#include "../utils.h"

/**
 * @brief Check the client requests and receive the message by the public FIFO.
 * 
 * @param np the file descriptor of the public FIFO
 * @param msg the message to receive from the client
 * @return 0 if request was read from the public FIFO, 1 otherwise
 */
int checkRequest(const int np, struct message *msg);

void getPrivateFifoName(char *fifo_name, struct message *msg);

int isPrivateFifoOpen(const char *fifo_name);

void sendAnswer(const int np, struct message msg);

#endif // SRC_CLIENT_FIFO_H_