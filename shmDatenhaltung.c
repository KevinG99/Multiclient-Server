#include "shmmemory.h"
#include <stdio.h>

char ausgabe[9192];
int put(char *key, char *value, int cfd, int shmID) {
    kv = (struct key_value *) shmat(shmID, NULL, 0);
    printf("1----\n");
    int non = 0;
    bzero(ausgabe, sizeof(ausgabe));
    for (int j = 0; j < kv->number; j++) {
        if (strcmp(kv[j].key, key) == 0) {
            strcpy(kv[j].key, key);
            strcpy(kv[j].value, value);
            sprintf(ausgabe, "PUT:%s:%s\n", kv[j].key, kv[j].value);
            write(cfd, ausgabe, strlen(ausgabe));
            non = 1;
            printf("2----\n");
            break;
        }
    }
    printf("3----\n");
    if (non == 0) {
        strcpy(kv[kv->number].key, key);
        strcpy(kv[kv->number].value, value);
        sprintf(ausgabe, "PUT:%s:%s\n", kv[kv->number].key, kv[kv->number].value);
        write(cfd, ausgabe, strlen(ausgabe));
        kv->number += 1;
        printf("4----\n");
    }
    return 0;
}


int get(char *key, int cfd, int shmID) {
    kv = (struct key_value *) shmat(shmID, NULL, 0);
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

int del(char *key, int cfd, int shmID) {
    kv = (struct key_value *) shmat(shmID, NULL, 0);
    int non = 0;
    bzero(ausgabe, sizeof(ausgabe));
    for (int j = 0; j < kv->number; j++) {
        if (strcmp(kv[j].key, key) == 0) {
            bzero(kv[j].value, sizeof(kv[j].value));
            bzero(kv[j].key, sizeof(kv[j].key));
            sprintf(ausgabe, "DEL:%s:key_deleted\n", key);
            write(cfd, ausgabe, strlen(ausgabe));
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


int keyvaluestore(int cfd, int shmID) {
    kv = (struct key_value *) shmat(shmID, NULL, 0);
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


