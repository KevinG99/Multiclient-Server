#include "main.h"


/*transaction semaphor erstellen in groesse von 10000 und dann mit control arbeiten
 * bei BEG veraendert er 99% davon sodass einer ueberbleibt fuer die befehle
 * bei END gibt er alle frei*/

/*if semctl != 2
    * if pid == sempid
* if accept => run
* */


int main() {
    signal(SIGINT, Signalhandler);
    /*Deklarationen*/
    int pid, t;
    char *eingabe, *eingabeHalter[BUFSIZE];
    char delimiter[] = "\n ";
    int rfd = erstelleSocket();
    int cfd;
//    int pi = 0;
    /////////////////////////////////////////////////////////
    /*Shared Memory*/
    int shmID = shmem();
    /*Semaphoren*/
    int semID1 = semGET();
    int semID2 = semGET2();
    printf("Prozess ID: %d Shared Memory ID: %d Semaphore ID's: %d und %d\n", getpid(), shmID, semID1, semID2);
    put("0", "0", 0, shmID);


    for (t=1; t<10; t++) {Eins[t]=Eins[0]; Null[t]=Null[0];}

//    marker[0] = 1;
//    marker2[0] = 1;
    t = semctl(semID1, 1, SETALL, Eins);
    if (t == -1) {
        fprintf(stderr, "Fehler beim setzen der Marker\n");
    }
    t = semctl(semID2, 10, SETALL, Null);
    if (t == -1) {
        fprintf(stderr, "Fehler beim setzen der Marker\n");
    }

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
//            pi++;

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
                    eingabeHalter[i++] = eingabe;
                    eingabe = strtok(NULL, delimiter);
                }
                ///////////////////////////////////////
                if (strncmp("quit", eingabeHalter[0], 4) == 0) {
                    close(cfd);
                    break;
                }

                Bedingung(eingabeHalter[0],
                          eingabeHalter[1],
                          eingabeHalter[2],
                          cfd, semID2, semID1, shmID, );

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