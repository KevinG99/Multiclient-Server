#ifndef BS_CODE_SHMEMORY_H
#define BS_CODE_SHMEMORY_H

#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <unistd.h>

#define BUFSIZE 1024
struct key_value {
    int number;
    char key[BUFSIZE];
    char value[BUFSIZE];
};

struct key_value *kv;

char ausgabe[9192];
int get(char *key, int cfd);
int put(char *key, char * value, int cfd, int msgID);
int del(char *key, int cfd, int msgID);
int keyvaluestore(int cfd);

int shmem();

#endif
