#include "main.h"

void signalhandler(int sig) {
    if (sig != SIGINT) {
        return;
    }
    int x = shmdt(kv);
    shmctl(shmem(), IPC_RMID, 0);
    semctl(semGET(), 0, IPC_RMID);
    if (x == 0) {
        printf("\nerfolgreich beendet\n");
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}