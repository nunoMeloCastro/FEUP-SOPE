#include "fifo.h"

#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#include "../macros.h"

int checkRequest(const int np, struct message *msg) {
    // Check if there is something to be read
    fd_set fds;

    FD_ZERO(&fds); // Clear FD set for select
    FD_SET(np, &fds);

    struct timeval timeout;
    timeout.tv_sec = SELECT_TIMEOUT_SEC;
    timeout.tv_usec = SELECT_TIMEOUT_USEC;
    select(np + 1, &fds, 0, 0, &timeout);

    // Public FIFO has data
    if (FD_ISSET(np, &fds)) {
        // We can read from public FIFO
        int res = read(np, msg, sizeof(struct message));
        if (res > 0) {
            writeLog(msg, RECVD);
            return 0;
        } else
            return FIFO_ERROR;
    }

    return 1;
}

void getPrivateFifoName(char *fifo_name, struct message *msg) {
    snprintf(fifo_name, MAX_STR_LEN, "/tmp/%i.%li", msg->pid, msg->tid);
}

int isPrivateFifoOpen(const char *fifo_name) {
    /*time_t start_time = time(NULL);
    int np;

    while (getElapsedTimeSecs(start_time) < MAX_WAIT_TIME){
        if ((np = open(fifo_name, O_WRONLY)) > 0) {
            return close(np);
        }
    }

    return FIFO_ERROR;*/
    return open(fifo_name, O_WRONLY);
}


void sendAnswer(const int np, struct message msg) {
    write(np, &msg, sizeof(struct message));
}
