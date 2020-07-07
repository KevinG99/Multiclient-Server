#include "shmmemory.h"
#include <stdio.h>

char ausgabe[9192];

int put(char *key, char *value, int cfd, int msgID) {
    int non = 0;

    bzero(ausgabe, sizeof(ausgabe));
    for (int j = 0; j < kv->number; j++) {
        if (strcmp(kv[j].key, key) == 0) {
            strcpy(kv[j].key, key);
            strcpy(kv[j].value, value);
            sprintf(ausgabe, "PUT:%s:%s\n", kv[j].key, kv[j].value);
            write(cfd, ausgabe, strlen(ausgabe));
            for (int i = 0; i < sharedSUB->counter; i++) {
                if (strcmp(kv[j].key, sharedSUB[i].key) == 0){
                    mess.mtype = sharedSUB[i].pid;
                    strcpy(mess.mtext, sharedSUB[i].key);
                    sharedSUB->haschanged = 1;
                    strcpy(sharedSUB->lastoperation, "PUT");
                    strcpy(sharedSUB->lastkey, key);
                    msgsnd(msgID, &mess, sizeof(mess), 0);
                }
            }
            non = 1;
            break;
        }
    }
    if (non == 0) {
        strcpy(kv[kv->number].key, key);
        strcpy(kv[kv->number].value, value);
        sprintf(ausgabe, "PUT:%s:%s\n", kv[kv->number].key, kv[kv->number].value);
        write(cfd, ausgabe, strlen(ausgabe));
        kv->number += 1;
    }
    return 0;
}


int get(char *key, int cfd) {
    int non = 0;
    bzero(ausgabe, sizeof(ausgabe));
    for (int j = 0; j < kv->number; j++) {
        if (strcmp(kv[j].key, key) == 0) {
            sprintf(ausgabe, "GET:%s:%s\n", kv[j].key, kv[j].value);
            write(cfd, ausgabe, strlen(ausgabe));
            non = 1;
            break;
        }
    }
    if (non == 0) {
        sprintf(ausgabe, "GET:%s:key_nonexistent\n", key);
        write(cfd, ausgabe, strlen(ausgabe));
    }
    return 0;
}

int del(char *key, int cfd, int msgID) {
    int non = 0;
    bzero(ausgabe, sizeof(ausgabe));
    for (int j = 0; j < kv->number; j++) {
        if (strcmp(kv[j].key, key) == 0) {
            bzero(kv[j].value, sizeof(kv[j].value));
            bzero(kv[j].key, sizeof(kv[j].key));
            sprintf(ausgabe, "DEL:%s:key_deleted\n", key);
            write(cfd, ausgabe, strlen(ausgabe));
            for (int i = 0; i < sharedSUB->counter; i++) {
                if (strcmp(key, sharedSUB[i].key) == 0){
                    mess.mtype = sharedSUB[i].pid;
                    strcpy(mess.mtext, sharedSUB[i].key);
                    sharedSUB->haschanged = 1;
                    strcpy(sharedSUB->lastoperation, "DEL");
                    strcpy(sharedSUB->lastkey, key);
                    msgsnd(msgID, &mess, sizeof(mess), 0);
                }
            }
            non = 1;
            break;
        }
    }
    if (non == 0) {
        sprintf(ausgabe, "DEL:%s:key_nonexistent\n", key);
        write(cfd, ausgabe, strlen(ausgabe));
    }
    return 0;
}


int keyvaluestore(int cfd) {
    bzero(ausgabe, sizeof(ausgabe));
    if (kv->number >= 0) {
        for (int i = 0; i < kv->number; i++) {
            sprintf(ausgabe, "Nummer:%d Key:%s Value:%s\n", i, kv[i].key, kv[i].value);
            write(cfd, ausgabe, strlen(ausgabe));
        }
    } else {
        sprintf(ausgabe, "Liste ist leer\n");
        write(cfd, ausgabe, strlen(ausgabe));
    }
    return 0;
}



