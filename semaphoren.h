#ifndef BS_CODE_BESSER_SEMAPHOREN_H
#define BS_CODE_BESSER_SEMAPHOREN_H

#define BUFSIZE 1024

#include <sys/sem.h>

int semGET();
int semGET2();
typedef int semaphor;
void up (semaphor id, int elem);
void down (semaphor id, int elem);


#endif
