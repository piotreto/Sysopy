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


#define BAKE_FILE "bake"
#define TABLE_FILE "table"
#define PIZZERIA_FILE "pizzeria"

#define PIZZERIA_ID 0
#define BAKE_ID 1
#define TABLE_ID 2

#define BAKE_SIZE 5
#define TABLE_SIZE 5

#define rand_number(_min, _max) (rand() %(_max - _min + 1)) + _min

#define rand_time ((rand() % (max_sleep - min_sleep + 1) + min_sleep) * 1000)


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

union semun {
    int val;
    struct semid_ds *buf;
    short *array;
};

int get_shared_block(char *filename, int size, int project_id);
void *attach_memory_block(char* filename, int size, int project_id);
void block(int semaphore_id, struct sembuf* sb);
void unlock(int semaphore_id, struct sembuf* sb);

char* timestamp();




#endif