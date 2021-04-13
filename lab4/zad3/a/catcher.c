#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int signal_1 = SIGUSR1;
int signal_2 = SIGUSR2;

int counter = 0;

char* mode;

void send_signals_back(pid_t dest) {
    if(strcmp(mode, "KILL") == 0 || strcmp(mode, "SIGRT") == 0) {
        for(int i = 0;i < counter;i++){
            kill(dest, signal_1);
        }
        kill(dest, signal_2);
    } else {
        union sigval val;
        for(int i = 0;i < counter;i++) {
            val.sival_int = i;
            sigqueue(dest, signal_1, val);
        }
        sigqueue(dest, signal_2, val);
    }
    exit(1);
}


void handle_signal1(int sig, siginfo_t *info, void* context) {
    counter += 1;
}

void handle_signal2(int sig, siginfo_t *info, void* context) {
    printf("Catcher received %d signals\n", counter);
    send_signals_back(info->si_pid);
}

void catch_signals() {

    sigset_t block, allowed;
    sigfillset(&block);
    sigfillset(&allowed);
    sigdelset(&allowed, signal_1);
    sigdelset(&allowed, signal_2);

    sigprocmask(SIG_SETMASK, &block, NULL);

    struct sigaction sa1;
    sigfillset(&sa1.sa_mask);
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = &handle_signal1;
    sigaction(signal_1, &sa1, NULL);

    struct sigaction sa2;
    sigfillset(&sa2.sa_mask);
    sa2.sa_flags = SA_SIGINFO;
    sa2.sa_sigaction = &handle_signal2;
    sigaction(signal_2, &sa2, NULL);

    while(1){
        sigsuspend(&allowed);
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Please give me one argument!\n");
        return 0;
    }
    mode = argv[1];
    printf("%d\n", getpid());
    if(strcmp(mode, "SIGRT") == 0) {
        signal_1 = SIGRTMIN+1;
        signal_2 = SIGRTMIN+2;
    }

    catch_signals();

    return 0;
}