#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

#define TEST SIGUSR1

void handle_sigusr1(int sig) {
    printf("Hello, im handling SIGUSR1, PID %d, PPID %d\n", getpid(), getppid());
}
void test_ignore() {
    printf("Im testing IGNORE option in main\n");
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(TEST, &sa, NULL);
    printf("Raising in parent\n");
    raise(TEST);
    printf("Nothing happend, so signal was ignored\n");
    char* args[] = {"./exec", "ignore",NULL};
    execvp(args[0], args);
    return;
}

void test_mask() {
    printf("I'm testing MASK option in main\n");
    sigset_t newmask; // sygnały do blokowania
    sigset_t oldmask; // aktualna maska
    sigemptyset(&newmask);
    sigaddset(&newmask, TEST);
    if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0) {
        printf("I couldnt mask signal \n");
    } else {
        printf("Signal is masked\n");
    }
    printf("Raising in main\n");
    raise(TEST);
    sigset_t pended_signals;
    sigpending(&pended_signals);
    if(sigismember(&pended_signals, TEST)) {
        printf("Signal was correctly masked\n");
    } else {
        printf("Signal wasnt masked\n");
    }
    char* args[] = {"./exec", "mask",NULL};
    execvp(args[0], args);
}

void test_pending() {
    printf("I'm testing PENDING option in main\n");
    sigset_t newmask; // sygnały do blokowania
    sigset_t oldmask; // aktualna maska
    sigemptyset(&newmask);
    sigaddset(&newmask, TEST);
    if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0) {
        printf("I couldnt mask signal \n");
    } else {
        printf("Signal is masked\n");
    }
    printf("Raising in main\n");
    raise(TEST);
    sigset_t pended_signals;
    printf("Checking in main\n");
    sigpending(&pended_signals);
    if(sigismember(&pended_signals, TEST)) {
        printf("Signal pending in a main\n");
    } else {
        printf("Signal isnt pedning in a main\n");
    }
    char* args[] = {"./exec", "pending",NULL};
    execvp(args[0], args);
    return;

}




int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Give me one argument!\n");
        return 0;
    }

    if(strcmp(argv[1], "ignore") == 0) {
        test_ignore();
    } else if(strcmp(argv[1], "mask") == 0) {
        test_mask();
    } else if(strcmp(argv[1], "pending") == 0) {
        test_pending();
    }
    return 0;
}