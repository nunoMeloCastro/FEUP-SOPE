#include "thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include "../macros.h"
#include "../utils.h"
#include "fifo.h"

// Used to identify each thread created by a unique number
int counter = 0;

// Control the access to the counter variable
sem_t cntMutex; 

// True when the client reaches timeout - terminate all pending requests
bool client_timeout = false;

void initSemaphore() {
    if (sem_init(&cntMutex, 1, 1) < 0) {
        perror("Semaphore initilization");
        exit(SEMAPHORE_ERROR);
    }
}

int getThreadNumber() {
    sem_wait(&cntMutex);
    int id = ++counter;
    sem_post(&cntMutex);

    return id;
}

void *pthr_create(void *arg) {
    time_t start_time = time(NULL);
    
    client_args client_args = *(struct client_args *)arg;
    
    // Control the access to counter (global variable) that is unique for each thread
    initSemaphore();

    // Create request threads
    int num_threads = ((client_args.nsecs * 1000)/MIN_SLEEP_TIME) + NUM_THREADS_OFFSET;
    const int NUM_THREADS = num_threads > MAX_NUM_THREADS ? MAX_NUM_THREADS : num_threads;
    pthread_t *threads = malloc(sizeof(pthread_t) * NUM_THREADS);

    int np_server = -1;
    int i = 0;

    while (getElapsedTimeSecs(start_time) < client_args.nsecs && i < NUM_THREADS) {
        // Check if Server FIFO is open
        np_server = open(client_args.fifo_name, O_WRONLY);
        
        if (np_server != -1) {
            if ((pthread_create(&threads[i], NULL, pthr_request, &np_server)) != 0) {
                fprintf(stderr, "Thread creation error\n");
                pthread_exit((void *)THREAD_ERROR);
            }
            i++;           
        }

        // Get random sleep time
        unsigned int seed = time(NULL);
        const int SLEEP_TIME = rand_r((unsigned int *)&seed) % (MAX_SLEEP_TIME + 1 - MIN_SLEEP_TIME) + MIN_SLEEP_TIME;

        // Create threads with an interval of some milliseconds between them
        sleepMillisecs(SLEEP_TIME); 
    }

    for (int j = 0; j < i; j++) {
        if (pthread_join(threads[j], NULL) != 0) {
            fprintf(stderr, "Thread join error\n");
            pthread_exit((void *)THREAD_ERROR);
        }
    }

    // Update client timeout since it has passed 'nsecs' seconds
    client_timeout = true;

    close(np_server);

    free(threads);

    return NULL;
}

void *pthr_request(void *arg) {
    int np_public_fifo = *(int *)arg;
    char priv_fifo[MAX_STR_LEN] = "";
    getPrivateFifoName(priv_fifo);

    struct message msg;
    
    msg.rid = getThreadNumber();
    int seed = msg.rid;
    msg.tskload = (rand_r((unsigned int *)&seed) % MAX_TSKLOAD) + MIN_TSKLOAD;
    msg.pid = getpid();
    msg.tid = pthread_self();
    msg.tskres = DEFAULT_TSKRES;

    writeLog(&msg, IWANT);
    makeRequest(np_public_fifo, msg);

    struct message msg_response;
    waitAnswer(priv_fifo, &msg_response);

    if (!client_timeout) {
        char *operation = (msg_response.tskres == DEFAULT_TSKRES) ? CLOSD : GOTRS;
        writeLog(&msg_response, operation);
    } else {
        writeLog(&msg_response, GAVUP);
    }

    return NULL;
}
