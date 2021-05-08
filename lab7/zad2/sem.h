#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define BAKE_FILE "/bake"
#define TABLE_FILE "/table"


#define BAKE_FILE_S "/bakes"
#define TABLE_FILE_S "/tables"



#define BAKE_SIZE 5
#define TABLE_SIZE 5


#define rand_number(_min, _max) (rand() %(_max - _min + 1)) + _min

typedef struct table {
    int place[TABLE_SIZE];
    int n_pizzas;
    int next_free;
    int to_take;
} table;

typedef struct bake {
    int place[BAKE_SIZE];
    int n_pizzas;
    int next_free;
} bake;

int get_shared_block(char *filename, int size);
void *attach_memory_block(char* filename, int size);


char* timestamp();



#endif