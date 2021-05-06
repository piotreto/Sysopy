#include "sem.h"

bake* pizzeria_bake;
table* pizzeria_table;


void clean_memory()
{
    if(shmdt(pizzeria_bake) == -1) {
        perror("Error while shmdt as a chief");
        exit(1);
    }
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

    pizzeria_bake = attach_memory_block(BAKE_FILE, 0, BAKE_ID);
    if (pizzeria_bake == (void*) -1)
    {
        perror("Error while attaching pizzeria_bake");
        exit(1);
    }

    pizzeria_table = attach_memory_block(TABLE_FILE, 0, TABLE_ID);
    if (pizzeria_table == (void*) -1)
    {
        perror("Error while attaching pizzeria_bake");
        exit(1);
    }

    struct sembuf op_bake;
    op_bake.sem_num = 0;
    op_bake.sem_flg = 0;
    op_bake.sem_op = -1;
    struct sembuf op_table;
    op_table.sem_num = 1;
    op_table.sem_flg = 0;
    op_table.sem_op = -1;
    while(1)
    {
        sleep(rand_number(1,3));
        int pizza_type = rand_number(1,9);
        sleep(2);
        printf("(%d %s) Przygotowuje pizze: %d\n", getpid(), timestamp(), pizza_type);
        block(semaphore_id, &op_bake);
        while(pizzeria_bake->n_pizzas == BAKE_SIZE)
        {
            unlock(semaphore_id, &op_bake);
            sleep(1);
            block(semaphore_id, &op_bake);
        }
        int idx_bake = pizzeria_bake->next_free;
        pizzeria_bake->place[idx_bake] = pizza_type;
        pizzeria_bake->next_free = (idx_bake + 1) % BAKE_SIZE;
        pizzeria_bake->n_pizzas += 1;
        printf("(%d %s) Dodalem pizze: %d. Liczba pizz w picecu: %d\n", getpid(), timestamp(), pizza_type, pizzeria_bake->n_pizzas);

        unlock(semaphore_id, &op_bake);

        sleep(5);

        block(semaphore_id, &op_bake);


        pizzeria_bake->place[idx_bake] = -1;
        pizzeria_bake->n_pizzas -= 1;

        printf("(%d %s) Wyjalem pizze: %d. Liczba pizz w picecu: %d\n", getpid(), timestamp(), pizza_type, pizzeria_bake->n_pizzas);

        unlock(semaphore_id, &op_bake);

        block(semaphore_id, &op_table);

        while(pizzeria_table->n_pizzas == TABLE_SIZE)
        {
            unlock(semaphore_id, &op_table);
            sleep(1);
            block(semaphore_id, &op_table);
        }

        int idx_table = pizzeria_table->next_free;
        pizzeria_table->place[idx_table] = pizza_type;
        pizzeria_table->n_pizzas += 1;
        pizzeria_table->next_free = (idx_table+1)%TABLE_SIZE;

        printf("(%d %s) Umieszczam pizze na stole: %d. Liczba pizz na stole: %d\n", getpid(), timestamp(), pizza_type, pizzeria_table->n_pizzas);

        unlock(semaphore_id, &op_table);

    }

}