#include "shmmemory.h"

#define SHMSIZE 4096

int shmem() {
    int shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | 0600);
    if (shmid == -1) {
        fprintf(stderr, "Fehler beim Erstellen des Shared Memories\n");
    }
    return shmid;
}
