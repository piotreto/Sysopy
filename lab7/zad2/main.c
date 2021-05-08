#include "sem.h"


int N;
int M;
pid_t *workers;
bake* pizzeria_bake;
table* pizzeria_table;

sem_t* semaphore_bake;
sem_t* semaphore_table;




void handler_SIGINT(int signum)
{
    for(int i = 0;i < (N+M);i++){
        kill(workers[i], SIGINT);
    }
    free(workers);
}

void clean_memory(char *filename, int size, void* block)
{
    if(munmap(block, size) == -1) {
        perror("Error while munmap");
        exit(1);
    }
    if(shm_unlink(filename) == -1) {
        perror("Error while shm_unlink in main");
        exit(1);
    }

}

void clean_semaphore(sem_t* semaphores_id, char* filename) {
    if(sem_close(semaphores_id) == -1)
    {
        perror("Error while closing sempahore");
        exit(1);
    }
    if(sem_unlink(filename) == -1)
    {
        perror("Error while unlinking sempahore");
        exit(1);
    }
}
void start()
{
    // creating sempaphore
    semaphore_bake = sem_open(BAKE_FILE_S, O_CREAT, 0660, 1);
    if (semaphore_bake == SEM_FAILED)
    {
        perror("Error while creating semaphore_bake");
        exit(1);
    }
    semaphore_table = sem_open(TABLE_FILE_S, O_CREAT, 0660, 1);
    if (semaphore_table == SEM_FAILED)
    {
        perror("Error while creating semaphore_table");
        exit(1);
    }


    pizzeria_bake = attach_memory_block(BAKE_FILE, sizeof(bake));
    if (pizzeria_bake == (void*) -1) {
        perror("Error while getting memory for bake");
        exit(1);
    }
    pizzeria_bake->n_pizzas = 0;
    pizzeria_bake->next_free = 0;

    pizzeria_table = attach_memory_block(TABLE_FILE, sizeof(table));
    if (pizzeria_table == (void*) -1) {
        perror("Error while getting memory for table");
        exit(1);
    }
    pizzeria_table->n_pizzas = 0;
    pizzeria_table->next_free = 0;
    pizzeria_table->to_take = 0;
    for(int i = 0;i < BAKE_SIZE;i++)
    {
        pizzeria_bake->place[i] = -1;
        pizzeria_table->place[i] = -1;
    }

    for(int i = 0;i < N;i++) {
        int pid = fork();
        workers[i] = pid;
        if (pid == 0)
        {
            workers[i] = pid;
            execlp("./chef", "./chef", NULL);
            exit(0);
        }
    }
    for(int i = N;i < N+M;i++) {
        int pid = fork();
        workers[i] = pid;
        if (pid == 0)
        {
            workers[i] = pid;
            execlp("./supplier", "./supplier", NULL);
            exit(0);
        }
    }
    for(int i = 0;i < (N+M);i++)
    {
        wait(NULL);
    }

    clean_memory(BAKE_FILE, sizeof(bake),pizzeria_bake);
    clean_memory(TABLE_FILE, sizeof(table),pizzeria_table);
    clean_semaphore(semaphore_bake, BAKE_FILE_S);
    clean_semaphore(semaphore_table, TABLE_FILE_S);

    
}
int main(int argc, char **argv)
{
    if (argc != 3) {
        perror("Błedna liczba argumentow! Musisz podac N i M");
        exit(1);
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);

    workers = (pid_t*)calloc(N + M, sizeof(pid_t));

    signal(SIGINT, handler_SIGINT);


    start();

    return 0;




}