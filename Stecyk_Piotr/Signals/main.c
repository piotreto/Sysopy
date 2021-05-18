#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void sighandler(int sig, siginfo_t *siginfo, void *context)
{
    printf("informations sig: %d, value: %d\n", sig ,siginfo->si_value.sival_int);
}

int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    //..........

    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigfillset(&block_mask);
    sigdelset(&block_mask, SIGUSR1);
    sigdelset(&block_mask, SIGUSR2);

    sigprocmask(SIG_BLOCK, &block_mask, NULL);
    int child = fork();
    if(child == 0) {
        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1 i SIGUSR2
        //zdefiniuj obsluge SIGUSR1 i SIGUSR2 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalami SIGUSR1 i SIGUSR2 wartosci
        pause();
    }
    else {
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
        int sig = atoi(argv[2]);
        union sigval sval;
        sval.sival_int = atoi(argv[1]);
        sigqueue(child, sig, sval);
    }

    return 0;
}
