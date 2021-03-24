#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {

    if (argc < 2){
        printf("You have to give me one parameter!");
    }
    int n = atoi(argv[1]);
    pid_t child_pid;
    pid_t parent = getpid();
    printf("This is mother of all %d\n", (int)parent);
    for(int i = 0;i < n;i++) {
        child_pid = fork();
        if(child_pid == 0) {
            printf("%d. Hello, im from %d, my mother is %d\n", i + 1, (int)getpid(), (int)getppid());
            return 0;
        }

    }
    wait(NULL);
    return 0;
}