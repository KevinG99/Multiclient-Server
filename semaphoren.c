#include "semaphoren.h"
#include <stdio.h>
#include <unistd.h>

int semGET() {
    int sem_id = semget (IPC_PRIVATE, 100, IPC_CREAT | IPC_EXCL | 0600);
    if (sem_id == -1) {
        fprintf(stderr, "Fehler beim Erstellen des Semaphores 1\n");
    }
    return sem_id;
}

int semGET2(){
    int sem_id = semget (IPC_PRIVATE, 1, IPC_CREAT  | IPC_EXCL | 0600);
    if (sem_id == -1) {
        fprintf(stderr, "Fehler beim Erstellen des Semaphores 2\n");
    }
    return sem_id;
}


void down (semaphor id, int elem) {
    struct sembuf sem_down;
    sem_down.sem_num = elem;
    sem_down.sem_op = -1;
    sem_down.sem_flg = 0;
    printf ("Semaphor down %i %i: %i. %i\n", id, elem, semctl(id, elem, GETVAL), getpid()); fflush(stdout);
    int t = semop (id, &sem_down, 1);
    if (t < 0 ){
        fprintf(stderr, "Fehler bei 'Down'\n");
    }
}

void up (semaphor id, int elem) {
    int t;
    struct sembuf sem_up;
    sem_up.sem_num = elem;
    sem_up.sem_op = +1;
    sem_up.sem_flg = 0;
    printf ("Semaphor up   %i %i: %i. %i\n", id, elem, semctl(id, elem, GETVAL), getpid());
    t = semop (id, &sem_up, 1);
    if (t < 0 ){
        fprintf(stderr, "Fehler bei 'UP'\n");
    }
}

