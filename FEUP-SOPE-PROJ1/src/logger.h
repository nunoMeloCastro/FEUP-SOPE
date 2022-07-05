#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PROC_CREAT "PROC_CREAT"
#define PROC_EXIT "PROC_EXIT"
#define SIGNAL_RECV "SIGNAL_RECV"
#define SIGNAL_SENT "SIGNAL_SENT"
#define FILE_MODF "FILE_MODF"

int openLogger();

int writeToLogger(int pid, const char *event, const char *info);

int closeLogger();

char* getInfoSig(const char *signal, const pid_t pid);

char* getGroupInfoSig(const char *signal, pid_t gid);

char *getInfoFModf(const char *fname);

#endif /* LOGGER_H_ */