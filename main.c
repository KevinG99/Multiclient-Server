#include "main.h"
#include <semaphore.h>

/*transaction semaphor erstellen in groesse von 10000 und dann mit control arbeiten
 * bei BEG veraendert er 99% davon sodass einer ueberbleibt fuer die befehle
 * bei END gibt er alle frei*/



int main() {
    signal(SIGINT, Signalhandler);
    /*Deklarationen*/
    int pid, t;
    char *eingabe, *inputBuffer[BUFSIZE];
    char delimiter[] = "\n ";

    int rfd = erstelleSocket();
    int cfd;
    /////////////////////////////////////////////////////////
    /*Shared Memory*/
    int shmID = shmem();
    /*Semaphoren*/
    int semID1 = semGET();
//    int semID2 = semGET2();
    printf("Prozess ID: %d Shared Memory ID: %d Semaphore ID's: %d und %d\n", getpid(), shmID, semID1, semID2);

    marker[0] = 1;
    t = semctl(semID1, 1, SETALL, marker);
    if (t == -1) {
        fprintf(stderr, "Fehler beim setzen der Marker\n");
    }

//    marker2[0] = 1;
//    t = semctl(semID2, 1, SETALL, marker2);
//    if (t == -1) {
//        fprintf(stderr, "Fehler beim setzen der Marker\n");
//    }


    sem_t semvar;

    sem_init(&semvar, 0, 0);



    ///////////////////////////////////////////////////////////////////
    while (ENDLOSSCHLEIFE) {
        // Verbindung eines Clients wird entgegengenommen
        cfd = accept(rfd, (struct sockaddr *) &client, &client_len);

        if (cfd < 0) {
            close(cfd);
            fprintf(stderr, "Verbindung abgebrochen\n");
            break;
        }
        ////////////////////////////////////////////////////////////
        /*Kindprozesse erzeugen*/
        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Error in new process creation\n");
        }


        if (pid == 0) {
            printf("CFD: %d | Prozess Nummer: %d\n", cfd, getpid());
            // Lesen von Daten, die der Client schickt
            bzero(input, sizeof(input));
            bytes_read = read(cfd, input, BUFSIZE);
            strncat(input, " ", strlen(" "));
            input[strcspn(input, "\r\n")] = 0;

            while (bytes_read > 0) {
                /*Eingabe halter fuellen*/
                eingabe = strtok(input, delimiter);
                int i = 0;
                while (eingabe != NULL) {
                    inputBuffer[i++] = eingabe;
                    eingabe = strtok(NULL, delimiter);
                }
                ///////////////////////////////////////
                if (strncmp("quit", inputBuffer[0], 4) == 0) {
                    close(cfd);
                    break;
                } else if (strncmp("BEG", inputBuffer[0], 3) == 0) {
//                    down(semID2, 0);
                    sem_wait(&semvar);
                    printf("blocked from %d\n", getpid());
                }
                conditions(inputBuffer[0],
                           inputBuffer[1],
                           inputBuffer[2],
                           cfd, semID1, shmID);

                if (strncmp("END", inputBuffer[0], 3) == 0) {
//                    up(semID2, 0);
                    sem_post(&semvar);
                    printf("open by %d\n", getpid());
                }

                bzero(input, sizeof(input));
                bytes_read = read(cfd, input, BUFSIZE);
                strncat(input, " ", strlen(" "));
                input[strcspn(input, "\r\n")] = 0;

                // Rendevouz Descriptor schließen
                close(rfd);
            }

        }
        close(cfd);
    }
    close(rfd);
    signal(SIGINT, Signalhandler);
}