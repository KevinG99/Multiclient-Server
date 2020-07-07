#ifndef BS_CODE_BESSER_MAIN_H
#define BS_CODE_BESSER_MAIN_H

#include "shmmemory.h"
#include "semaphoren.h"
#include "pubsub.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

#include <netinet/in.h>


#define ENDLOSSCHLEIFE 1
#define PORT 5678

void signalhandler(int sig);

struct sockaddr_in server;
struct sockaddr_in client;
socklen_t client_len;
char input[BUFSIZE];
int bytes_read;
int erstelleSocket();



int beg();
int end();

unsigned short marker[2];


#endif //BS_CODE_BESSER_MAIN_H
