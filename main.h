#ifndef BS_CODE_BESSER_MAIN_H
#define BS_CODE_BESSER_MAIN_H

#include "shmmemory.h"
#include "semaphoren.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

#include <netinet/in.h>


#define ENDLOSSCHLEIFE 1
#define PORT 5678

void Signalhandler(int sig);

struct sockaddr_in server;
struct sockaddr_in client; // Socketadresse eines Clients
socklen_t client_len; // Länge der Client-Daten
char input[BUFSIZE]; // Daten vom Client an den Server
int bytes_read; // Anzahl der Bytes, die der Client geschickt hat
int erstelleSocket();

void conditions(char *eingabehalter1,
                char *eingabehalter2,
                char *eingabehalter3,
                int cfd, int semID1, int shmID);

unsigned short marker[2], marker2[2];


#endif //BS_CODE_BESSER_MAIN_H
