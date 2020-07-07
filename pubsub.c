#include "pubsub.h"


/* In Warteschlange schreiben */

int getMSG() {
    int msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0644); //Server erzeugt eine Message Queue
    if (msg_id == -1) {
        fprintf(stderr, "Fehler beim Erstellen einer Warteschlange\n");
        exit(1);
    }
    return msg_id;
}

int subSHM() {
    int shmid = shmget(IPC_PRIVATE, 5*sizeof(struct sub), IPC_CREAT | 0600);
    if (shmid == -1) {
        fprintf(stderr, "Fehler beim Erstellen des Shared Memories\n");
    }
    return shmid;
}




