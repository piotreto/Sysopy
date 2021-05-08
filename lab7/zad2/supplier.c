#include "sem.h"

#include "sem.h"

table* pizzeria_table;


sem_t* semaphore_table;


void clean_memory()
{
    if(munmap(pizzeria_table, sizeof(pizzeria_table)) == -1) {
        perror("Error while munmap");
        exit(1);
    }
    
}

void clean_semaphores() {
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
    
    semaphore_table = sem_open(TABLE_FILE_S, 0);
    if (semaphore_table == SEM_FAILED)
    {
        perror("Error while creating semaphore_pizzeria");
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
        sem_wait(semaphore_table);

        while(pizzeria_table->n_pizzas <= 0)
        {
            sem_post(semaphore_table);
            sleep(1);
            sem_wait(semaphore_table);
        }
        int idx_from = pizzeria_table->to_take;
        int pizza_type = pizzeria_table->place[idx_from];
        pizzeria_table->n_pizzas -= 1;
        pizzeria_table->to_take = (idx_from+1) % TABLE_SIZE;

        printf("(%d %s) Pobieram pizze: %d. Liczba pizz na stole: %d\n", getpid(), timestamp(), pizza_type, pizzeria_table->n_pizzas);
        
        sem_post(semaphore_table);

        sleep(4);

        printf("(%d %s) Dostarczam pizze: %d\n", getpid(), timestamp(), pizza_type);

        sleep(4);

    }

}
