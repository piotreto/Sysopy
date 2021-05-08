#include "sem.h"

bake* pizzeria_bake;
table* pizzeria_table;


sem_t* semaphore_bake;
sem_t* semaphore_table;


void clean_memory()
{
    if(munmap(pizzeria_bake, sizeof(pizzeria_bake)) == -1) {
        perror("Error while munmap");
        exit(1);
    }
    if(munmap(pizzeria_table, sizeof(pizzeria_table)) == -1) {
        perror("Error while munmap");
        exit(1);
    }
    
}

void clean_semaphores() {
    if(sem_close(semaphore_bake) == -1)
    {
        perror("Error while closing sempahore");
        exit(1);
    }
    if(sem_close(semaphore_table) == -1)
    {
        perror("Error while closing sempahore");
        exit(1);
    }
}
void handler_SIGINT(int signum)
{
    clean_memory();
    clean_semaphores();
    exit(0);
}


int main(int argc, char **argv)
{
    srand(time(NULL) ^ (getpid()<<16));
    signal(SIGINT, handler_SIGINT);
    
    semaphore_bake = sem_open(BAKE_FILE_S, 0);
    if (semaphore_bake == SEM_FAILED)
    {
        perror("Error while creating semaphore_bake");
        exit(1);
    }
    semaphore_table = sem_open(TABLE_FILE_S, 0);
    if (semaphore_table == SEM_FAILED)
    {
        perror("Error while creating semaphore_pizzeria");
        exit(1);
    }

    pizzeria_bake = attach_memory_block(BAKE_FILE, sizeof(pizzeria_bake));
    if (pizzeria_bake == (void*) -1)
    {
        perror("Error while attaching pizzeria_bake");
        exit(1);
    }

    pizzeria_table = attach_memory_block(TABLE_FILE, sizeof(pizzeria_table));
    if (pizzeria_table == (void*) -1)
    {
        perror("Error while attaching pizzeria_bake");
        exit(1);
    }

    while(1)
    {
        sleep(rand_number(1,3));
        int pizza_type = rand_number(1,9);
        sleep(2);
        printf("(%d %s) Przygotowuje pizze: %d\n", getpid(), timestamp(), pizza_type);
        sem_wait(semaphore_bake);
        while(pizzeria_bake->n_pizzas == BAKE_SIZE)
        {
            sem_post(semaphore_bake);
            sleep(1);
            sem_wait(semaphore_bake);
        }
        int idx_bake = pizzeria_bake->next_free;
        pizzeria_bake->place[idx_bake] = pizza_type;
        pizzeria_bake->next_free = (idx_bake + 1) % BAKE_SIZE;
        pizzeria_bake->n_pizzas += 1;
        printf("(%d %s) Dodalem pizze: %d. Liczba pizz w picecu: %d\n", getpid(), timestamp(), pizza_type, pizzeria_bake->n_pizzas);

        sem_post(semaphore_bake);

        sleep(5);

        sem_wait(semaphore_bake);


        pizzeria_bake->place[idx_bake] = -1;
        pizzeria_bake->n_pizzas -= 1;

        printf("(%d %s) Wyjalem pizze: %d. Liczba pizz w picecu: %d\n", getpid(), timestamp(), pizza_type, pizzeria_bake->n_pizzas);

        sem_post(semaphore_bake);

        sem_wait(semaphore_table);

        while(pizzeria_table->n_pizzas == TABLE_SIZE)
        {
            sem_post(semaphore_table);
            sleep(1);
            sem_wait(semaphore_table);
        }

        int idx_table = pizzeria_table->next_free;
        pizzeria_table->place[idx_table] = pizza_type;
        pizzeria_table->n_pizzas += 1;
        pizzeria_table->next_free = (idx_table+1)%TABLE_SIZE;

        printf("(%d %s) Umieszczam pizze na stole: %d. Liczba pizz na stole: %d\n", getpid(), timestamp(), pizza_type, pizzeria_table->n_pizzas);

         sem_post(semaphore_table);

    }

}