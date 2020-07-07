#include "main.h"

int state = 0;

int beg() {
    state = 1;
    return 0;
}

int end() {
    state = 0;
    return 0;
}

void cleanString(char *str) {
    for (int (j) = 0; (j) < strlen(str); ++(j)) {
        if ((str[j] == '\n') || (str[j]) == '\r') {
            str[j] = '\0';
        }
    }
}

char transaktion[BUFSIZE];
int boolChanged = 0;


int main() {
    signal(SIGINT, signalhandler);
    /*Deklarationen*/
    int pid, t;
    char *eingabe, *inputBuffer[20] = {NULL};

    int rfd = erstelleSocket();
    int cfd;
    /////////////////////////////////////////////////////////
    /*Shared Memory*/
    int shmID = shmem();
    kv = (struct key_value *) shmat(shmID, NULL, 0);

    /*Message Queue*/
    int shmSUB_id = subSHM();
    sharedSUB = (struct sub *) shmat(shmSUB_id, NULL, 0);
    int msg_id = getMSG();
    printf("Message Shared Memory: %d und die msgID: %d\n", shmSUB_id, msg_id);

    /*Semaphoren*/
    semaphor semID1 = semGET();
    semaphor semID2 = semGET2();
    printf("Prozess ID: %d Shared Memory ID: %d Semaphore ID's: %d und %d\n", getpid(), shmID, semID1, semID2);

    marker[0] = 1;
    t = semctl(semID1, 1, SETALL, marker);
    if (t < 0) {
        fprintf(stderr, "Fehler beim setzen der Marker\n");
    }

    t = semctl(semID2, 1, SETALL, marker);
    if (t < 0) {
        fprintf(stderr, "Fehler beim setzen der Marker\n");
    }

    put("0", "0", 0, shmID);

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


            while (1) {
                bzero(input, sizeof(input));
                while (1) {
                    if (boolChanged != 0 && sharedSUB->haschanged == 1) {
                        if (msgrcv(msg_id, &mess, sizeof(mess), getpid(), 0) != -1) {
                            for (int j = 0; j <= sharedSUB->counter; j++) {
                                for (int i = 0; i <= kv->number; i++) {
                                    if (strcmp(kv[i].key, sharedSUB[j].key) == 0) {
                                        if (strcmp(sharedSUB->lastoperation, "PUT") == 0) {
                                            sprintf(ausgabe, "PUT:%s:%s\n", kv[i].key, kv[i].value);
                                            write(cfd, ausgabe, strlen(ausgabe));
                                            sharedSUB->haschanged = 0;
                                        }
                                        if (strcmp(sharedSUB->lastoperation, "DEL")==0){
                                            sprintf(ausgabe, "DEL:%s:key_deleted\n", sharedSUB->lastkey);
                                            write(cfd, ausgabe, strlen(ausgabe));
                                            sharedSUB->haschanged = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    bytes_read = recv(cfd, input, BUFSIZE, MSG_DONTWAIT);
                    if (bytes_read > -1) {
                        break;
                    }
                }


                for (int j = 0; j < 10; j++) {
                    inputBuffer[j] = NULL;
                }
                eingabe = NULL;
                eingabe = strtok(input, " ");

                int i = 0;
                while (eingabe != NULL) {
                    inputBuffer[i++] = eingabe;
                    eingabe = strtok(NULL, " ");
                }
                ///////////////////////////////////////


                if (strncmp("quit", inputBuffer[0], 4) == 0) {
                    close(cfd);
                    if (state != 1) {
                        up(semID2, 0);
                    }
                    break;
                }

                if (state != 1) {
                    printf("down\n");
                    down(semID2, 0);
                }


                bzero(transaktion, sizeof(transaktion));
                down(semID1, 0);

                if (strncmp("PUT", inputBuffer[0], 3) == 0) {
                    cleanString(inputBuffer[1]);
                    cleanString(inputBuffer[2]);
                    put(inputBuffer[2], inputBuffer[2], cfd, msg_id);
                } else if (strcmp("GET", inputBuffer[0]) == 0) {
                    cleanString(inputBuffer[1]);
                    get(inputBuffer[1], cfd);
                } else if (strncmp("DEL", inputBuffer[0], 3) == 0) {
                    cleanString(inputBuffer[1]);
                    del(inputBuffer[1], cfd, msg_id);
                } else if (strncmp("alles", inputBuffer[0], 5) == 0) {
                    keyvaluestore(cfd);
                } else if (strncmp("BEG", inputBuffer[0], 3) == 0) {
                    beg();
                    sprintf(transaktion, "BEG:Transaktion mit Client (PID=%d) aktiviert\n\n", getpid());
                    write(cfd, transaktion, sizeof(transaktion));
                } else if (strncmp("END", inputBuffer[0], 3) == 0) {
                    end();
                    sprintf(transaktion, "END:Transaktion mit Client (PID=%d) wurde beendet\n\n", getpid());
                    write(cfd, transaktion, sizeof(transaktion));
                } else if (strncmp("SUB", inputBuffer[0], 3) == 0) {
                    long subPid = getpid();
                    int non = 0;
                    for (int j = 0; j < kv->number; j++) {
                        if (strcmp(kv[j].key, inputBuffer[1]) == 0) {
                            non = 1;
                            sprintf(transaktion, "SUB:%s:%s\n", inputBuffer[1], inputBuffer[2]);
                            write(cfd, transaktion, sizeof(transaktion));
                            strcpy(sharedSUB[sharedSUB->counter].key, kv[j].key);
                            sharedSUB[sharedSUB->counter].pid = subPid;
                            sharedSUB->counter++;
                            boolChanged = 1;
                            break;
                        }
                    }
                    if (non == 0) {
                        sprintf(transaktion, "SUB:key_nonexistent\n");
                        write(cfd, transaktion, sizeof(transaktion));
                    }

                } else {
                    write(cfd, "cmd_nonexistent\n", strlen("cmd_nonexistent\n"));
                }
                up(semID1, 0);


                if (state != 1) {
                    printf("up\n");
                    up(semID2, 0);
                }

                // Rendevouz Descriptor schließen
//                close(rfd);
            }
        }
        close(cfd);
    }
    close(rfd);
    signal(SIGINT, signalhandler);
}