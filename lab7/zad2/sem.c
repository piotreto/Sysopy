#include "sem.h"

int get_shared_block(char *filename, int size)
{
    int fd = shm_open(filename, O_RDWR | O_CREAT, 0760);
    ftruncate(fd, size);
    return fd;
}
void *attach_memory_block(char *filename, int size)
{
    int shared_block_id = get_shared_block(filename, size);

    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_block_id, 0);
}


char * timestamp() {
    time_t now = time(NULL);
    char * time = asctime(gmtime(&now));
    time[strlen(time)-1] = '\0';
    return time;
}