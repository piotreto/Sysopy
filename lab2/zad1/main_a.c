#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    
    while(count == -1 && readed == tmp_size) { // if readed < tmp_size it means we got end of file, MonkaS
        free(tmp_string);
        fseek(fptr, -readed ,SEEK_CUR);
        tmp_size *= 2;
 
        tmp_string = (char*)calloc(tmp_size, sizeof(char));
        readed = fread(tmp_string, sizeof(char), tmp_size, fptr);
        count = is_line(tmp_string, readed);
    }
    
    char *result;
    if (count == -1) // we are on last line, that doesnt contain \n
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

int main(int argc, char **argv)
{
    FILE *fptr1;
    FILE *fptr2;
    if (argc == 3)
    {
        fptr1 = fopen(argv[1], "r");
        fptr2 = fopen(argv[2], "r");
    }
    else
    {
        printf("%s", "You have to give me two files!\n");
        char file1[256];
        char file2[256];
        printf("First file: ");
        scanf("%s", file1);
        printf("Second file: ");
        scanf("%s", file2);
        fptr1 = fopen(file1, "r");
        fptr2 = fopen(file2, "r");
    }
    if (fptr1 && fptr2) // using fread and fwrite
    {
        printf("Using fread and fwrite\n");
        while (feof(fptr1) == 0 || feof(fptr2) == 0)
        {
            if (feof(fptr1) == 0)
            {
                char *line = getline_f(fptr1);
                printf("%s", line);
                free(line);
            }
            if (feof(fptr2) == 0)
            {
                char *line = getline_f(fptr2);
                printf("%s", line);
                free(line);
            }
        }
        fclose(fptr1);
        fclose(fptr2);
    }
    else
    {
        printf("%s", "Unfortunately, something went wrong with opening your files, try again");
        return 0;
    }
    return 0;
}