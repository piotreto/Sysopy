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
    printf("Im testing IGNORE option in exec\n");
    printf("Raising in exec\n");
    raise(TEST);
    printf("Nothing happend, so signal was ignored\n");
}

void test_mask() {
    printf("I'm testing MASK option in exec\n");
    printf("Raising in exec\n");
    raise(TEST);
    sigset_t pended_signals;
    sigpending(&pended_signals);
    if(sigismember(&pended_signals, TEST)) {
        printf("Signal was masked in exec\n");
    } else {
        printf("Signal wasnt masked in exec\n");
    }
    return;
}

void test_pending() {
    printf("I'm testing PENDING option in exec\n");
    sigset_t pended_signals;
    sigpending(&pended_signals);
    printf("Checking in exec\n");
    if(sigismember(&pended_signals, TEST)) {
        printf("Signal pending in exec\n");
    } else {
        printf("Signal isnt pedning in exec\n");
    }
    return;
}




int main(int argc, char** argv) {
    
    if(strcmp(argv[1], "ignore") == 0) {
        test_ignore();
    } else if(strcmp(argv[1], "mask") == 0) {
        test_mask();
    } else if(strcmp(argv[1], "pending") == 0) {
        test_pending();
    }
    return 0;
}