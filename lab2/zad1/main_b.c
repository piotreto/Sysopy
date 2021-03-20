#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
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
char *getline_s(int in)
{
    size_t tmp_size = 256;
    char *tmp_string = (char *)calloc(tmp_size, sizeof(char));
    size_t readed = read(in, tmp_string, tmp_size);
    int count = is_line(tmp_string, readed);
    while(count == -1 && readed == tmp_size) { // if readed < tmp_size it means we got end of file, MonkaS
        free(tmp_string);
        lseek(in, -readed ,SEEK_CUR);
        tmp_size *= 2;
 
        tmp_string = (char*)calloc(tmp_size, sizeof(char));
        readed = read(in, tmp_string, tmp_size);
        count = is_line(tmp_string, readed);
    }
    
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
    int fptr2;
    if (argc == 3)
    {
        fptr1 = open(argv[1], O_RDONLY);
        fptr2 = open(argv[2], O_RDONLY);
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
        fptr1 = open(file1, O_RDONLY);
        fptr2 = open(file2, O_RDONLY);
    }
    if (fptr1 != -1 && fptr2 != -1) // using fread and fwrite
    {
        printf("Using read and write\n");
        char* line1;
        char* line2;
        line1 = getline_s(fptr1);
        line2 = getline_s(fptr2);
        while (line1  != NULL || line2 != NULL)
        {
            if (line1 != NULL)
            {
                printf("%s", line1);
                free(line1);
                line1 = getline_s(fptr1);
            }
            if (line2 != NULL)
            {
                printf("%s", line2);
                free(line2);
                line2 = getline_s(fptr2);
            }
        }
    }
    else
    {
        printf("%s", "Unfortunately, something went wrong with opening your files, try again");
        return 0;
    }
    return 0;
}
