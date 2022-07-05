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

#include "../lib/lib.h"
#include "../macros.h"
#include "../utils.h"
#include "fifo.h"
#include "buffer.h"

// Flag to notify consumer to stop waiting from results in buffer
bool server_timeout = false;

void *pthr_receiver(void *arg) {
    // Receives the nsecs and the public FIFO name
    server_args main_args = *(struct server_args *)arg;

    // Open Public FIFO
    int np;
    while ((np = open(main_args.fifo_name, O_RDONLY)) < 0); 

    // Allocate space for producer threads
    pthread_t *threads = malloc(sizeof(pthread_t) * MAX_NUM_THREADS);

    // Producer args
    struct message msg;
    bool attend_request = true;  // Indicates either the producer thread as created in time and must attend the request or not

    time_t start_time = time(NULL);
    time_t current_time;
    int check_request, i = 0;
    
    // Elapsed time smaller than nsecs - OR - Queued orders still exist
    while ((i < MAX_NUM_THREADS)/*(((current_time = getElapsedTimeSecs(start_time)) < main_args.nsecs) || ((check_request = checkRequest(np, &msg)) == 0)) && */) {
        current_time = getElapsedTimeSecs(start_time);
        check_request = checkRequest(np, &msg);

        if ((current_time < main_args.nsecs) || (check_request == 0)) {

            // Passed server operating time
            if (current_time >= main_args.nsecs) 
                attend_request = false;
            
            // Request was read from public FIFO - create corresponding producer thread
            if (check_request == 0) {
                producer_args producer_args;
                producer_args.msg = &msg;
                producer_args.attend_request = attend_request;

                // Send to producer thread the struct message and the attend flag
                if ((pthread_create(&threads[i], NULL, pthr_producer, &producer_args)) != 0) {
                    fprintf(stderr, "Producer Thread creation error\n");
                    pthread_exit((void *)THREAD_ERROR);
                }

                i++; 
            }          
        } else break;
    }

    for (int j = 0; j < i; j++) {
        if (pthread_join(threads[j], NULL) != 0) {
            fprintf(stderr, "Producer Thread join error\n");
            pthread_exit((void *)THREAD_ERROR);
        }
    }

    server_timeout = true;

    
    close(np);
    unlink(main_args.fifo_name);

    free(threads);
    return NULL;
}


void *pthr_producer(void *arg) {
    producer_args prod_arg = *(struct producer_args *)arg;

    if (prod_arg.attend_request) {
        // invoca tarefa em B, espera pelo resultado e altera na mensagem
        //printf("tskload: %d\n" , prod_arg.msg->tskload);
        
        int task_result = task(prod_arg.msg->tskload);
        writeLog(prod_arg.msg, TSKEX);

        prod_arg.msg->tskres = task_result;
    } 

    // coloca resultado no buffer a mensagem 
    // no caso de !attend_request esta sem alteracao (res = -1)
    writeOnBuffer(prod_arg.msg);

    return NULL;
}

void *pthr_consumer(void *arg) {
    struct message msg;
    char priv_fifo[MAX_STR_LEN] = "";
    
    while (readFromBuffer(&msg) == 0) {
        getPrivateFifoName(priv_fifo, &msg); 
        printMessage(msg);


        int np = open(priv_fifo,O_WRONLY);
        printf("np = %d\n", np);

        if (np > 0) {
            if (msg.tskres != DEFAULT_TSKRES) 
                writeLog(&msg, TSKDN);
            else 
                writeLog(&msg, LATE);
        } else {
            writeLog(&msg, FAILD);
            close(np);
            continue;
        }

        // Update message
        msg.pid = getpid();
        msg.tid = pthread_self();

        sendAnswer(np, msg);
        close(np);
    }

    return NULL;
}
