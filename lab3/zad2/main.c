#define  _GNU_SOURCE
#include <stdio.h>
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <time.h>

typedef struct time_struct
{
    double real;
    double user;
    double sys;
} time_struct;

void display_file(pair *files)
{
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, files->tmp_merged) != -1)
    {
        printf("%s", line);
    }
    free(line);
}
void parse_sequnce(int n, char **seq)
{
    for (int i = 0; i < n; i++)
    {
        pid_t child_pid;
        child_pid = fork();
        if (child_pid == 0)
        {
            pair *files = (pair *)calloc(1, sizeof(pair));
            files->file1 = strtok(seq[i], ":");
            files->file2 = strtok(NULL, "");
            merge_files(files);
            display_file(files);
            fclose(files->tmp_merged);
            free(files);
            exit(0);
        }
    }
}

void time_test(char *file, int n)
{

    struct tms test_times[2]; //usr and sys
    clock_t test_times_real[2];

    // merging elements
    times(&test_times[0]);
    test_times_real[0] = clock();
    fflush(stdout);
    for (int i = 0; i < n; i++)
    {
        pid_t child_pid = 0;
        child_pid = fork();
        if (child_pid == 0)
        {
            pair *files = (pair *)calloc(1, sizeof(pair));
            files->file1 = file;
            files->file2 = file;
            merge_files(files);
            fclose(files->tmp_merged);
            free(files);
            exit(0);
        }
    }
    for(int i = 0;i < n;i++){
        wait(NULL);
    }

    times(&test_times[1]);
    test_times_real[1] = clock();

    time_struct time;

    time.real = (double)(test_times_real[1] - test_times_real[0]) / CLOCKS_PER_SEC;
    time.sys = (double)(test_times[1].tms_stime - test_times[0].tms_stime) / sysconf(_SC_CLK_TCK);
    time.user = (double)(test_times[1].tms_utime - test_times[0].tms_utime) / sysconf(_SC_CLK_TCK);


    printf("\nMERGE\n");
    printf("REAL         USER        SYSTEM\n");
    printf("%lfs    %lfs   %lfs\n", time.real, time.user, time.sys);
}

int main(int argc, char **argv)
{
    if (strcmp(argv[1], "test") == 0)
    {
         puts("-----TEST 1-----");
        puts("----20 row----");
        puts("");
        puts("----5 pairs----");
        time_test("test1.txt", 5);
        puts("");
        puts("----200 pairs----");
        time_test("test1.txt", 200);
        puts("");
        puts("----1000 pairs----");
        time_test("test1.txt", 1000);

        puts("");
        puts("");
        
        puts("-----TEST 2-----");
        puts("----400 rows----");
        puts("");
        puts("----5 pairs----");
        time_test("test2.txt", 5);
        puts("");
        puts("----200 pairs----");
        time_test("test2.txt", 200);
        puts("");
        puts("----1000 pairs----");
        time_test("test2.txt", 1000);

        puts("");
        puts("");

        puts("-----TEST 3-----");
        puts("----1000 rows----");
        puts("");
        puts("----5 pairs----");
        time_test("test3.txt", 5);
        puts("");
        puts("----200 pairs----");
        time_test("test3.txt", 200);
        puts("");
        puts("----1000 pairs----");
        time_test("test3.txt", 1000);

        return 0;
    }
    else if (argc < 2 || atoi(argv[1]) == 0)
    {
        printf("Cmmon make, give me more parameters!");
        return 0;
    }
    else
    {
        int n = atoi(argv[1]);
        parse_sequnce(n, argv + 2);

        wait(NULL);
        return 0;
    }
}