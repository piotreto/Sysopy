#include "sem.h"


int N;
int M;
pid_t *workers;
bake* pizzeria_bake;
table* pizzeria_table;



void handler_SIGINT(int signum)
{
    for(int i = 0;i < (N+M);i++){
        kill(workers[i], SIGINT);
    }
}

void clean_memory(char *filename, int size, int project_id, void* block)
{

    int shared_block_id = get_shared_block(filename, size, project_id);
    if(shmdt(block) == -1) {
        perror("Error while shmdt");
        exit(1);
    }
    if(shmctl(shared_block_id, IPC_RMID, NULL) == -1) {
        perror("Error while shmctl in main");
        exit(1);
    }

}

void clean_semaphores(int semaphores_id, int n) {
    if(semctl(semaphores_id, 0, IPC_RMID, NULL) == -1)
    {
        perror("Error while removing semaphore");
        exit(1);
    }
}
void start()
{

    // getting key for semaphore
    key_t key;
    key = ftok(PIZZERIA_FILE, PIZZERIA_ID);

    // creating sempaphore
    int semaphore_id = semget(key, 2, 0666 | IPC_CREAT);

    if(semaphore_id == -1) {
        perror("Error while creating semaphore.\n");
        exit(1);
    }


    // initializating semaphore
    union semun arg;
    arg.val = 1;

    if(semctl(semaphore_id, 0, SETVAL, arg) == -1) {
        perror("Error while initializing semaphore.");
        exit(1);
    }

    if(semctl(semaphore_id, 1, SETVAL, arg) == -1) {
        perror("Error while initializing semaphore.");
        exit(1);
    }

    pizzeria_bake = attach_memory_block(BAKE_FILE, sizeof(bake), BAKE_ID);
    if (pizzeria_bake == (void*) -1) {
        perror("Error while getting memory for bake");
        exit(1);
    }
    pizzeria_bake->n_pizzas = 0;
    pizzeria_bake->next_free = 0;

    pizzeria_table = attach_memory_block(TABLE_FILE, sizeof(table), TABLE_ID);
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
            execlp("./chief", "./chief", NULL);
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

    clean_memory(BAKE_FILE, 0, BAKE_ID, pizzeria_bake);
    clean_memory(TABLE_FILE, 0, TABLE_ID, pizzeria_table);
    clean_semaphores(semaphore_id, 2);

    
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