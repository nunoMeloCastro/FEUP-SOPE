#include "buffer.h"

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
#include "thread.h"

extern bool server_timeout;

struct message *buffer;
int buffer_size;

// Control the access to the counter variable
sem_t mutexRead, mutexWrite, mutexFill; 
int cntRead, cntWrite, cntFill = 0;


void initSemaphore() {
    if ((sem_init(&mutexRead, 1, 0) < 0) || (sem_init(&mutexWrite, 1, 1) < 0) || (sem_init(&mutexFill, 1, 1) < 0)) {
        perror("Semaphore initilization");
        exit(SEMAPHORE_ERROR);
    }
}

void bufferConstructor(const int size) {
    cntRead = 0;
    cntWrite = 0;
    initSemaphore();

    buffer_size = size;
    buffer = (struct message *) malloc(buffer_size * sizeof(*buffer));

    // Make sure all fields are 
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = (struct message) {0, 0, 0, 0, 0};
    }
}

void bufferDestructor() {
    sem_close(&mutexRead);
    sem_close(&mutexWrite);
    sem_close(&mutexFill);
    free(buffer);
}

void writeOnBuffer(struct message *msg) {
    sem_wait(&mutexWrite);

    const struct message emptyMessage = {0, 0, 0, 0, 0};
    // enquanto esse espaço nao estiver vazio
    while (compareMessages(&buffer[cntWrite], &emptyMessage) != 0);

    sem_wait(&mutexFill);
        memcpy(&buffer[cntWrite], msg, sizeof(struct message));
        cntFill++;
    sem_post(&mutexFill);
    
    cntWrite++;
    if (cntWrite == buffer_size) cntWrite = 0;

    sem_post(&mutexWrite);
}

int readFromBuffer(struct message *msg) {
    /*
    // Checks if mutexRead is close
    int sem_value;
    bool abort = false;
    while ((sem_getvalue(&mutexRead, &sem_value) == 0) && (sem_value == 0) && (!abort)) {   
        // In case there is a server timeout
        if (server_timeout) {
            abort = true;
        }    
    }

    if (abort) return 1;

    sem_wait(&mutexWrite);

    memcpy(msg, &buffer[cntRead], sizeof(struct message));
    buffer[cntRead]= (struct message) {0, 0, 0, 0, 0};

    cntRead++;

    if (cntRead == buffer_size) cntRead = 0;

    if (cntRead == cntWrite) sem_wait(&mutexRead);

    sem_post(&mutexWrite);
    */
    
    bool abort = false;
    while (cntFill == 0) {    
        if (abort) return 1;

        if (server_timeout) {
            //sleep(1);
            abort = true;
        }   
    }

    sem_wait(&mutexFill);
        memcpy(msg, &buffer[cntRead], sizeof(struct message));
        // clean buffer
        buffer[cntRead]= (struct message) {0, 0, 0, 0, 0};
        cntFill--;
    sem_post(&mutexFill);

    cntRead++;
    if (cntRead == buffer_size) cntRead = 0;
    
    return 0;
}
