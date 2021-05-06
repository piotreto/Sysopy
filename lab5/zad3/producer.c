#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>



int main(int argc, char** argv) {
    if (argc != 5) {
        printf("Niepoprawna liczba argumentow!\n");
        return 1;
    }
    char* fifo_path = argv[1];
    char* data_path = argv[3];
    int row = atoi(argv[2]);
    int N = atoi(argv[4]);

    int fifo = open(fifo_path, O_WRONLY);
    FILE* data = fopen(data_path, "r");

    char* products = (char*)calloc(N, sizeof(char));


    while(fread(products, sizeof(char), N, data) == N) {
        sleep(1);
        flock(fifo, LOCK_EX);
        write(fifo, &row, sizeof(int));
        write(fifo, products, N*sizeof(char));
        flock(fifo, LOCK_UN);
    }

    free(products);
    close(fifo);
    fclose(data);

    return 0;

}