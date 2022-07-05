#include "fifo.h"

#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../macros.h"

extern bool client_timeout;

int isPublicFifoOpen(const char *fifo_name) {
    time_t start_time = time(NULL);
    int np;

    while (getElapsedTimeSecs(start_time) < MAX_WAIT_TIME){
        if ((np = open(fifo_name, O_WRONLY)) > 0) {
            return close(np);
        }
    }

    return ERROR;
} 

void getPrivateFifoName(char *fifo_name) {
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    snprintf(fifo_name, MAX_STR_LEN, "/tmp/%i.%li", pid, tid);
}

void makeRequest(const int np, struct message msg) {
    write(np, &msg, sizeof(struct message));
}

void waitAnswer(const char *fifo_name, struct message *msg) {
    int np;
    unlink(fifo_name);

    if (mkfifo(fifo_name, 0622) < 0) { 
        perror("Make private fifo"); 
        pthread_exit((void *)FIFO_ERROR); 
    }

    while ((np = open(fifo_name, O_RDONLY)) < 0); 

    // On success, the number of bytes read is returned
    while ((read(np, msg, sizeof(struct message)) <= 0) && (!client_timeout));

    close(np);

    if (unlink(fifo_name) < 0) {
        perror("Remove private fifo"); 
        pthread_exit((void *)FIFO_ERROR); 
    }
}
