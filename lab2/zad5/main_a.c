#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

typedef struct time_struct {
    double real;
    double user;
    double sys;
} time_struct;

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
        int count = 0;
        char ch;
        for(int i = 0;line[i] != '\0';i++) {
            if(count < 50 || line[i] == '\n') {
                fwrite(&line[i], sizeof(char), 1, fptr);
                count += 1;
            } else {
                ch = '\n';
                fwrite(&ch, sizeof(char), 1, fptr);
                count = 0;
                i -= 1;
            }
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
    if(argc == 2 && strcmp(argv[1], "test") == 0) {
        struct tms test_times[2]; //usr and sys
        clock_t test_times_real[2];

        times(&test_times[0]);
        test_times_real[0] = clock();
        char* file1 = "../duze_pliki/5000rows.txt";
        char* file2 = "5000rows_out.txt";
        break_lines(file1,file2);
        times(&test_times[1]);
        test_times_real[1] = clock();
        time_struct time_tab;
        time_tab.real = (double) (test_times_real[1] - test_times_real[0]) / CLOCKS_PER_SEC;
        time_tab.sys = (double) (test_times[1].tms_stime - test_times[0].tms_stime) / sysconf(_SC_CLK_TCK);
        time_tab.user = (double) (test_times[1].tms_utime - test_times[0].tms_utime) / sysconf(_SC_CLK_TCK);
        printf("REAL         USER        SYSTEM\n");
        printf("%lfs    %lfs   %lfs\n", time_tab.real, time_tab.user, time_tab.sys);
        return 0;
    }
    if(argc < 3) {
        printf("You have to give me two files!\n");
        return 0;
    }
    break_lines(argv[1], argv[2]);
    return 0;
}