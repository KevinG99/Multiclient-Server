#ifndef BS_CODE_PUBSUB_H
#define BS_CODE_PUBSUB_H

#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include "main.h"
#define BUFSIZE 1024


struct text_message {
    long mtype;             // Typ als long
    char mtext[BUFSIZE];    //Daten als char array von der Größe 1024
};

struct text_message mess;

struct sub{
    int counter;
    long pid;
    char key[BUFSIZE];
    char lastoperation[BUFSIZE];
    char lastkey[BUFSIZE];
    int haschanged;
};

struct sub *sharedSUB;

int subSHM();
int getMSG();


#endif


