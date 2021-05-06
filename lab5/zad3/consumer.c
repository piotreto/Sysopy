#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>



int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Niepoprawna liczba argumentow\n");
        return 1;
    }
    char* fifo_path = argv[1];
    char* data_path = argv[2];
    int N = atoi(argv[3]);

    int fifo = open(fifo_path, O_RDONLY);
    FILE* data = fopen(data_path, "w+");

    char* line = (char*)calloc(N, sizeof(char));
    int row;
    while (read(fifo, &row, sizeof(int)))
    {
        flock(fifo, LOCK_EX);
        fseek(data, 0, SEEK_SET);
        read(fifo, line, N * sizeof(char));
        int cnt = 0;
        char ch;
        while(fread(&ch, sizeof(char), 1, data)) {
            if (ch == '\n') {
                cnt += 1;
            }
            if(cnt == row) {
                fseek(data, -1, SEEK_CUR);
                char COPY[256*10];
                int move = fread(COPY, sizeof(char), 256*10, data);
                fseek(data, -1 * move, SEEK_CUR);
                fwrite(line, sizeof(char), N, data);
                fwrite(COPY, sizeof(char), move, data);
                break;
            }
        }
        if(cnt < row) {
            for(int i = cnt+1;i <= row;i++) {
                char ROW_INFO[256];
                sprintf(ROW_INFO, "Received materials at row %d: \n", i);
                fwrite(ROW_INFO, sizeof(char), strlen(ROW_INFO), data);
            }
            fseek(data, -1, SEEK_CUR);
            fwrite(line, sizeof(char), N, data);
            fwrite("\n", sizeof(char), 1, data);
        }
        flock(fifo, LOCK_UN);
    }
    free(line);
    close(fifo);
    fclose(data);

    return 0;

    
}