#define _GNU_SOURCE

#include "utils.h"

#include <ctype.h>
#include <unistd.h>
#include <stdio.h>

int isNumber(const char *str) {
    for (int i = 0; str[i] != 0; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

time_t getElapsedTimeSecs(time_t start) {
    return time(NULL) - start;
}

int sleepMillisecs(int num){
    return usleep(num * 1000); 
}

void writeLog(const struct message *msg, const char* oper){    
    // inst ; i ; t ; pid ; tid ; res ; oper 
    fprintf(stdout, "%ld ; %d ; %d ; %d ; %ld ; %d ; %s\n", time(0), msg->rid, msg->tskload, msg->pid, msg->tid, msg->tskres, oper);
}
