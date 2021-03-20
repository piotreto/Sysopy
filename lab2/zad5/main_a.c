#include <stdio.h>
#include <stdlib.h>
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
char *getline_f(FILE *fptr)
{
    size_t tmp_size = 256;
    char *tmp_string = (char *)calloc(tmp_size, sizeof(char));
    size_t readed = fread(tmp_string, sizeof(char), tmp_size, fptr);
    int count = is_line(tmp_string, readed);

    char *result;
    if (readed == 0)
    {
        result = NULL;
    }
    else if (count == -1) // we are on last line, that doesnt contain \n
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
        fseek(fptr, -(readed - count), SEEK_CUR);
    }
    free(tmp_string);
    return result;
}

void break_n_print(FILE* fptr, char* line){
    if(strlen(line) <= 51) // 51, because we dont count new line char
    {
        fwrite(line, sizeof(char), strlen(line), fptr);
        return;
    } else {
        char* copy = (char*)calloc(52, sizeof(char));
        strncpy(copy, line, 50);
        copy[50] = '\n';
        copy[51] = '\0';
        fwrite(copy, sizeof(char), strlen(copy), fptr);
        free(copy);
        for(int i = 50;i < strlen(line);i++) {
            fwrite(&line[i], sizeof(char), 1, fptr);
        }
    }
}

void break_lines(char *file1, char *file2)
{
    FILE *fptr1 = fopen(file1, "r");
    FILE *fptr2 = fopen(file2, "w");
    if (fptr1 && fptr2)
    {
        char *line;
        while ((line = getline_f(fptr1)) != NULL)
        {
            break_n_print(fptr2, line);
            free(line);
        }
        fclose(fptr1);
        fclose(fptr2);
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