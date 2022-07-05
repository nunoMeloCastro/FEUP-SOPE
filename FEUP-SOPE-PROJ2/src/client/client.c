#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

#include "../macros.h"
#include "../utils.h"
#include "input_check.h"
#include "fifo.h"
#include "thread.h"

int main(int argc, char *argv[], char *envp[]) {
    setbuf(stdout, NULL);

    // Check arguments
    if (isValidInput(argc, argv) != 0) {
        usageNotRight();
        exit(INPUT_ERROR);
    }

    // Check if public FIFO is open
    if (isPublicFifoOpen(argv[argc - 1]) != 0){
        fprintf(stderr, "Accessing FIFO error\n");
        exit(FIFO_ERROR);
    }

    // Save client arguments
    client_args client_args;
    client_args.nsecs = atoi(argv[2]);
    snprintf(client_args.fifo_name, MAX_STR_LEN, "%s", argv[3]);
    
    // Create main thread (c0)
    pthread_t main_thread;
    if ((pthread_create(&main_thread, NULL, pthr_create, &client_args)) != 0) {
        fprintf(stderr, "Main thread creation error\n");
        exit(THREAD_ERROR);
    }
    
    if (pthread_join(main_thread, NULL) != 0) {
        fprintf(stderr, "Main thread join error\n");
        exit(THREAD_ERROR);
    }

    return 0;
}
