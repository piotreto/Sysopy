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
int n_signals;

char* mode;

void handle_signal1(int sig, siginfo_t *info, void* context) {
    if(strcmp(mode, "SIGQUEUE") == 0) {
        printf("Received signal, index: %d\n", info->si_value.sival_int);
    }
    counter += 1;
}

void handle_signal2(int sig, siginfo_t *info, void* context) {
    printf("We should get %d signals, we got %d\n", n_signals, counter);
    exit(1);
}


void send_signals(pid_t dest){
    if(strcmp(mode, "KILL") == 0 || strcmp(mode, "SIGRT") == 0) {
        for(int i = 0;i < n_signals;i++){
            kill(dest, signal_1);
        }
        kill(dest, signal_2);
    } 
    else {
        union sigval val;
        for(int i = 0;i < n_signals;i++) {
            val.sival_int = i;
            sigqueue(dest, signal_1, val);
        }
        sigqueue(dest, signal_2, val);
    }
}


int main(int argc, char** argv) {
    if(argc != 4) {
        printf("You have to give 3 arguments!\n");
        return 0;
    }
    pid_t dest = atoi(argv[1]);
    n_signals = atoi(argv[2]);
    mode = argv[3];

    if(strcmp(mode, "SIGRT") == 0) {
        signal_1 = SIGRTMIN+1;
        signal_2 = SIGRTMIN+2;

    }

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

    send_signals(dest);

    while(1){
        sigsuspend(&allowed);
    }

}