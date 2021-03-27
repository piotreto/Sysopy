#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


const int PATH_N = 1000;

int check_file(char* path, char* pattern) {
    FILE* fptr = fopen(path, "r");
    if(fptr != NULL)
    {
        char* buf = NULL;
        size_t len = 0;
        while(getline(&buf, &len, fptr) != -1) {
            if(strstr(buf, pattern) != NULL) return 1;
        }
        fclose(fptr);
    }
    return -1;
}
void readDirectory(char *base_path, char *pattern, int max_depth, int depth) {
    int status = 0;
    char path[PATH_N];
    struct dirent *dp;
    DIR *dir = opendir(base_path);
    if(dir == NULL) {
        exit(0);
    }
    while ((dp = readdir(dir)) != NULL)
    {
        strcpy(path, base_path);
        strcat(path, "/");
        strcat(path, dp->d_name);

        if (dp->d_type == DT_DIR && depth < max_depth && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if(fork() == 0){
                readDirectory(path, pattern, max_depth, depth + 1);
                exit(0);
            }
        } else if(dp->d_type == DT_REG && strstr(dp->d_name, ".txt") != NULL) 
        {
            if(check_file(path, pattern) == 1) {
                printf("%s\tPID %d\t PPID %d\n", path, getpid(), getppid());
            }
        }
    }
    closedir(dir);
    while(wait(&status) > 0);
}

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        printf("You have to give me 3 arguments!\n");
        return 0;
    }
    fflush(stdout);
    readDirectory(argv[1], argv[2], atoi(argv[3]),0);
}