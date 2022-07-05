#define _GNU_SOURCE

#include "utils.h"

#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

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

// If return == 0 => messages are equal
int compareMessages(const struct message* msg1, const struct message* msg2) {
    return memcmp(msg1, msg2, sizeof(struct message));
}

void writeLog(const struct message *msg, const char* oper){    
    // inst ; i ; t ; pid ; tid ; res ; oper 
    fprintf(stdout, "%ld ; %d ; %d ; %d ; %ld ; %d ; %s\n", time(0), msg->rid, msg->tskload, getpid(), pthread_self(), msg->tskres, oper);
}

void printMessage(struct message msg) {
    printf("rid = %d, pid = %d, tid = %d, tskload = %d, tskres = %d\n", msg.rid, msg.pid, msg.rid, msg.tskload, msg.tskres);
}
