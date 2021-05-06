#include "sem.h"

int get_shared_block(char *filename, int size, int project_id)
{
    key_t key;


    key = ftok(filename, project_id);
    if (key == -1)
    {
        perror("Error while ftok\n");
        exit(1);
    }
    return shmget(key, size, 0644 | IPC_CREAT);
}
void *attach_memory_block(char *filename, int size, int project_id)
{
    int shared_block_id = get_shared_block(filename, size, project_id);
    char *result;
    if (shared_block_id == -1)
    {
        perror("Error while creating memory block");
        exit(1);
    }
    result = shmat(shared_block_id, NULL, 0);
    if (result == (void*)-1)
    {
        perror("Error while shmat");
        exit(1);
    }

    return result;
}



void block(int semaphore_id, struct sembuf* sb)
{
    sb->sem_op = -1;
    if(semop(semaphore_id, sb, 1) == -1) {
        printf("Error while semop\n");
        exit(1);
    }

}
void unlock(int semaphore_id, struct sembuf* sb)
{
    sb->sem_op = 1;
    if(semop(semaphore_id, sb, 1) == -1) {
        printf("Error while semop\n");
        exit(1);
    }
}


char * timestamp() {
    time_t now = time(NULL);
    char * time = asctime(gmtime(&now));
    time[strlen(time)-1] = '\0';
    return time;
}