#include "sem.h"

#include "sem.h"

table* pizzeria_table;


void clean_memory()
{
    if(shmdt(pizzeria_table) == -1) {
        perror("Error while shmdt as a chief");
        exit(1);
    }
    
}


void handler_SIGINT(int signum)
{
    clean_memory();
    exit(0);
}



int main(int argc, char **argv)
{
    srand(time(NULL) ^ (getpid()<<16));
    signal(SIGINT, handler_SIGINT);
    key_t key;
    key = ftok(PIZZERIA_FILE, PIZZERIA_ID);
    if (key == -1)
    {
        perror("Error while getting key.");
        exit(1);
    }
    int semaphore_id = semget(key, 0, 0);
    if (semaphore_id == -1)
    {
        perror("Error while getting semaphore id\n");
        exit(1);
    }

    pizzeria_table = attach_memory_block(TABLE_FILE, 0, TABLE_ID);
    if (pizzeria_table == (void*) -1)
    {
        perror("Error while attaching pizzeria_bake");
        exit(1);
    }

    struct sembuf op_table;
    op_table.sem_num = 1;
    op_table.sem_flg = 0;
    op_table.sem_op = -1;
    while(1)
    {
        sleep(rand_number(1,3));
        block(semaphore_id, &op_table);

        while(pizzeria_table->n_pizzas <= 0)
        {
            unlock(semaphore_id, &op_table);
            sleep(1);
            block(semaphore_id, &op_table);
        }
        int idx_from = pizzeria_table->to_take;
        int pizza_type = pizzeria_table->place[idx_from];
        pizzeria_table->n_pizzas -= 1;
        pizzeria_table->to_take = (idx_from+1) % TABLE_SIZE;

        printf("(%d %s) Pobieram pizze: %d. Liczba pizz na stole: %d\n", getpid(), timestamp(), pizza_type, pizzeria_table->n_pizzas);
        
        unlock(semaphore_id, &op_table);

        sleep(4);

        printf("(%d %s) Dostarczam pizze: %d\n", getpid(), timestamp(), pizza_type);

        sleep(4);

    }

}
