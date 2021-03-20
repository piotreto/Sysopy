#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int contains_c(char *string, char ch) {
    for(int i = 0;string[i] != '\0'; i++) {
        if(string[i] == ch) return 1;
    }
    return -1;
}

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





int main(int argc, char **argv)
{
    int fptr1;
    char ch;
    if (argc == 3)
    {
        fptr1 = open(argv[1], O_RDONLY);
        ch = argv[2][0];
    }
    else
    {
        return 0;
    }
    if (fptr1 != -1) // using fread and fwrite
    {
        printf("Using read and write\n");
        char* line;
        while ((line = getline_s(fptr1)) != NULL)
        {
            if(contains_c(line, ch) == 1) printf("%s", line);
            free(line);
        }
    }
    else
    {
        printf("%s", "Unfortunately, something went wrong with opening your files, try again");
        return 0;
    }
    return 0;
}