#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>


void handler_extended(int sig, siginfo_t *info, void*context) {
    printf("I got signal, PID: %d, PPID: %d\n", getpid(), getppid());
    printf("Signal number: %d\n", info->si_signo);
    printf("Signal code: %d\n", info->si_code);
    printf("Real user ID of sending process: %d\n", info->si_uid);

}
void handler_classic(int sig) {
    printf("I got signal, PID: %d, PPID: %d\n", getpid(), getppid());
}


void test1() {
    printf("TESTINF SA_SIGINFO");
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handler_extended;
    sigaction(SIGUSR1, &sa, NULL);
    raise(SIGUSR1);
}
void test2() {

    printf("TESTING SA_RESETHAND FLAG\n");
    struct sigaction sa;
    sa.sa_handler = &handler_classic;
    sigaction(SIGCHLD, &sa, NULL);
    int pid = fork();
    if (pid == 0) {
        while(1);
    } else{
        sleep(1);
        kill(pid, SIGKILL);
    }
    wait(NULL);
    pid = fork();
    if (pid == 0) {
        while(1);
    } else{
        sleep(1);
        kill(pid, SIGKILL);
    }
}
// jak widzimy, kiedy uzyjemy flagi SA_RESETHAND nasz handler wywołuje się tylko po pierwszym razie otrzymania sygnału
// pozniej resetowany jest na handler defaultowy, czyli po prostu ignorujący

void test3() {
    printf("TESTING SA_NOCLDSTOP FLAG\n");
    struct sigaction sa;
    sa.sa_handler = &handler_classic;
    sa.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
    int pid = fork();
    if(pid == 0) {
        while(1);
    } else {
        kill(pid, SIGSTOP);
        sleep(1);
        kill(pid, SIGCONT);
        kill(pid, SIGKILL);
    }
    wait(NULL);
}
// as we can see, when using NOCLDSTOP, when fork is stopped or continue we dont receive signal from child process
// only when child is died

int main(int argc, char** argv) {
    test1();
    test2();
    test3();

}