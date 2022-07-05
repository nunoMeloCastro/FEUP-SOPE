#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include "../macros.h"
#include "../utils.h"
#include "input_check.h"
#include "thread.h"
#include "buffer.h"

int main(int argc, char *argv[], char *envp[]) {
    setbuf(stdout, NULL);

    // Check arguments
    if (isValidInput(argc, argv) != 0) {
        usageNotRight();
        exit(INPUT_ERROR);
    }

    // Criar a fifo publica com o nome passado em argv[5]
    const char *fifo_name = argv[5];
    if (mkfifo(fifo_name, 0666) != 0) {
        fprintf(stderr, "Public fifo creation error\n");
        return FIFO_ERROR;
    }
    
    // abrir o armazém
    int buffsz = atoi(argv[4]);
    bufferConstructor(buffsz);

    // Save server arguments
    server_args server_args;
    server_args.nsecs = atoi(argv[2]);
    snprintf(server_args.fifo_name, MAX_STR_LEN, "%s", fifo_name);
    
    // Create main thread (s0)
    pthread_t main_thread;
    if ((pthread_create(&main_thread, NULL, pthr_receiver, &server_args)) != 0) {
        fprintf(stderr, "Main thread creation error\n");
        exit(THREAD_ERROR);
    }
    
    // Create consumer thread (sc)
    pthread_t consumer_thread;
    if ((pthread_create(&consumer_thread, NULL, pthr_consumer, NULL)) != 0) {
        fprintf(stderr, "Consumer thread creation error\n");
        exit(THREAD_ERROR);
    }
    
    if (pthread_join(main_thread, NULL) != 0) {
        fprintf(stderr, "Main thread join error\n");
        exit(THREAD_ERROR);
    }   

    if (pthread_join(consumer_thread, NULL) != 0) {
        fprintf(stderr, "Consumer thread join error\n");
        exit(THREAD_ERROR);
    }   


    bufferDestructor();

    return 0;
}
