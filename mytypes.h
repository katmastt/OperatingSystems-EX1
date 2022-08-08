#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define MEM_KEY (key_t) 1234
#define SEM_KEY1 (key_t) 1230
#define SEM_KEY2 (key_t) 1231
#define SEM_KEY3 (key_t) 1232
#define SEM_KEY4 (key_t) 1233

#define LINES 20

struct message {
    int pid;
    char* line;
};

struct shared_mem {
    struct message in_ds;
    struct message out_ds;
};

union semun {
    int val;
};

int* creation(int);
int producers_proc(void);
int consumer_proc(int, int);

