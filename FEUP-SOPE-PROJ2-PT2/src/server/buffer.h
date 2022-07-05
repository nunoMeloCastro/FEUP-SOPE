#ifndef SRC_SERVER_BUFFER_H_
#define SRC_SERVER_BUFFER_H_

#include "../utils.h"

void bufferConstructor(const int size);

void bufferDestructor();

void writeOnBuffer(struct message *msg);

int readFromBuffer(struct message *msg);

#endif // SRC_SERVER_BUFFER_H_