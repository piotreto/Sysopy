#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>


int is_line(char *string, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (string[i] == '\n')
        {
            return i + 1;
        }
    }
    return -1;
}

char *getline_s(int in)
{
    size_t tmp_size = 256;
    char *tmp_string = (char *)calloc(tmp_size, sizeof(char));
    size_t readed = read(in, tmp_string, tmp_size);
    int count = is_line(tmp_string, readed);
    char *result;
    if (readed == 0)
    {
        result = NULL;
    }
    else if (count == -1) // we get last line in file without \n char
    {
        result = (char *)calloc(readed + 1, sizeof(char));
        strncpy(result, tmp_string, readed);
        result[readed] = '\0';
    }
    else
    {
        result = (char *)calloc(count + 1, sizeof(char));
        strncpy(result, tmp_string, count);
        result[count] = '\0';
        lseek(in, -(readed - count), SEEK_CUR);
    }
    free(tmp_string);
    return result;
}

void break_n_print(int fptr, char* line){
    if(strlen(line) <= 51) // 51, because we dont count new line char
    {
        write(fptr, line,strlen(line));
        return;
    } else {
        char* copy = (char*)calloc(52, sizeof(char));
        strncpy(copy, line, 50);
        copy[50] = '\n';
        copy[51] = '\0';
        write(fptr, copy,strlen(copy));
        free(copy);
        for(int i = 50;i < strlen(line);i++) {
            write(fptr, &line[i], 1);
        }
    }
}

void break_lines(char *file1, char *file2)
{
    int fptr1 = open(file1,O_RDONLY);
    int fptr2 = open(file2, O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    if (fptr1 != -1 && fptr2 != -1)
    {
        char *line;
        while ((line = getline_s(fptr1)) != NULL)
        {
            break_n_print(fptr2, line);
            free(line);
        }
        close(fptr1);
        close(fptr2);
    } else {
        printf("I couldnt open your files, something went wrong.\n");
        return;
    }
    return;
}

int main(int argc, char** argv){
    if(argc < 3) {
        printf("You have to give me two files!\n");
        return 0;
    }
    break_lines(argv[1], argv[2]);
    return 0;
}