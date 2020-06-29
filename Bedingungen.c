#include "main.h"

void Bedingung(char *eingabehalter1,
               char *eingabehalter2,
               char *eingabehalter3,
               int cfd, int semID1, int semID2, int shmID) {
    semaphor mutex = semID1;
    semaphor mutex2 = semID2;




    if (strncmp("BEG", eingabehalter1, 3) == 0) {
        /*sperre prozesse der anderen*/
        up(mutex, 0);
        printf("geblockt fuer andere\n");
    } else if (strncmp("PUT", eingabehalter1, 3) == 0) {
        down(mutex, 0);
        put(eingabehalter2, eingabehalter3, cfd, shmID);
        up(mutex, 0);
    } else if (strncmp("GET", eingabehalter1, 3) == 0) {
        down(mutex, 0);
        get(eingabehalter2, cfd, shmID);
        up(mutex, 0);
    } else if (strncmp("DEL", eingabehalter1, 3) == 0) {
        down(mutex, 0);
        del(eingabehalter2, cfd, shmID);
        up(mutex, 0);
    } else if (strncmp("alles", eingabehalter1, 5) == 0) {
        down(mutex, 0);
        keyvaluestore(cfd, shmID);
        up(mutex, 0);
    } else if (strncmp("END", eingabehalter1, 3) == 0) {
        /*gebe die prozesse wieder frei*/
        up(mutex, 0);
        printf("freigegeben fuer andere\n");
    } else if (strncmp("SUB", eingabehalter1, 3) == 0) {
        printf("kommt noch\n");
    } else {
        write(cfd, "cmd_nonexistent\n", strlen("cmd_nonexistent\n"));
    }
}