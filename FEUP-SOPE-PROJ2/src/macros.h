#ifndef SRC_MACROS_H_
#define SRC_MACROS_H_

#define MAX_STR_LEN 255

#define MAX_WAIT_TIME 5 // in seconds
#define MIN_SLEEP_TIME 50   // in milliseconds
#define MAX_SLEEP_TIME 100  // in milliseconds

#define NUM_THREADS_OFFSET 5
#define MAX_NUM_THREADS 5000

#define MIN_TSKLOAD 1
#define MAX_TSKLOAD 9
#define DEFAULT_TSKRES -1

#define ERROR -1
#define INPUT_ERROR 1
#define FIFO_ERROR 2
#define THREAD_ERROR 3
#define SEMAPHORE_ERROR 4

#define IWANT "IWANT"
#define GOTRS "GOTRS"
#define CLOSD "CLOSD"
#define GAVUP "GAVUP"

#endif // SRC_MACROS_H_
