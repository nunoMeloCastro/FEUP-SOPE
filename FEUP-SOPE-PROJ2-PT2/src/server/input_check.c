#include "input_check.h"

#include <string.h>
#include <stdio.h>

#include "../macros.h"
#include "../utils.h"

void usageNotRight() {
    printf("\nusage: ./s <-t nsecs> [-l bufsz] <fifoname>\n\n");

    // Define the description better
    printf("\tnsecs: approximated time (in seconds) that the program should run\n");
    printf("\tbufsz: size of the buffer to be used to store the results of the requests\n");
    printf("\tfifoname: (absolute or relative) name of the public communication channel (FIFO) between the Client and the Server\n\n");
}

int isValidInput(int argc, char *argv[]) {
    if (argc != 6)
        return ERROR;

    if (strcmp(argv[1], "-t") != 0)
        return ERROR;

    if (!isNumber(argv[2]))
        return ERROR;

    if (strcmp(argv[3], "-l") != 0)
        return ERROR;

    if (!isNumber(argv[4]))
        return ERROR;

    return 0;
}
