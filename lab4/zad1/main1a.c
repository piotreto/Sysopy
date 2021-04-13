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

void test_handler() {
    printf("Im testing HANDLER option\n");
    struct sigaction sa;
    sa.sa_handler = &handle_sigusr1;
    sigaction(TEST, &sa, NULL);
    int pid = fork();
    if(pid == -1) {
        printf("Fork doesnt work\n");
        raise(TEST);
        exit(1);
    }
    if(pid == 0) {
        // we are in child process
        printf("Raising in child\n");
        raise(TEST);
        exit(1);
    } else {
        printf("Raising in parent\n");
        raise(TEST);
    }
    wait(NULL);
}

void test_ignore() {
    printf("Im testing IGNORE option\n");
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(TEST, &sa, NULL);
    int pid = fork();
    if(pid == -1) {
        printf("Fork doesnt work\n");
        raise(TEST);
        exit(1);
    }
    if(pid == 0) {
        // we are in child process
        printf("Raising in child\n");
        raise(TEST);
        printf("Nothing happend, so signal was ignored\n");
        exit(1);
    } else {
        printf("Raising in parent\n");
        raise(TEST);
        printf("Nothing happend, so signal was ignored\n");
    }
    wait(NULL);
}

void test_mask() {
    printf("I'm testing MASK option\n");
    sigset_t newmask; // sygnały do blokowania
    sigset_t oldmask; // aktualna maska
    sigemptyset(&newmask);
    sigaddset(&newmask, TEST);
    if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0) {
        printf("I couldnt mask signal \n");
    } else {
        printf("Signal is masked\n");
    }
    int pid = fork();
    if(pid == -1) {
        printf("Fork doesnt work\n");
        exit(1);
    }
    if(pid == 0) {
        printf("Raising in child\n");
        raise(TEST);
        sigset_t pended_signals;
        sigpending(&pended_signals);
        if(sigismember(&pended_signals, TEST)) {
            printf("Signal was correctly masked\n");
        } else {
            printf("Signal wasnt masked\n");
        }
        exit(0);
    } else {
        printf("Raising in parent\n");
        raise(TEST);
        sigset_t pended_signals;
        sigpending(&pended_signals);
        if(sigismember(&pended_signals, TEST)) {
            printf("Signal was correctly masked\n");
        } else {
            printf("Signal wasnt masked\n");
        }
    }
    wait(NULL);
    return;
}

void test_pending() {
    printf("I'm testing PENDING option\n");
    sigset_t newmask; // sygnały do blokowania
    sigset_t oldmask; // aktualna maska
    sigemptyset(&newmask);
    sigaddset(&newmask, TEST);
    if(sigprocmask(SIG_SETMASK, &newmask, &oldmask) < 0) {
        printf("I couldnt mask signal \n");
    } else {
        printf("Signal is masked\n");
    }
    raise(TEST);
    sigset_t pended_signals;
    int pid = fork();
    if(pid == -1) {
        printf("Fork doesnt work\n");
        exit(1);
    }
    if(pid == 0) {
        printf("Checking in child\n");
        sigpending(&pended_signals);
        if(sigismember(&pended_signals, TEST)) {
            printf("Signal pending in a child\n");
        } else {
            printf("Signal isnt pedning in a child\n");
        }
        exit(0);
    } else {
        printf("Checking in parent\n");
        sigpending(&pended_signals);
        if(sigismember(&pended_signals, TEST)) {
            printf("Signal pending in a parent\n");
        } else {
            printf("Signal isnt pedning in a parent\n");
        }
    }
    wait(NULL);
    return;

}




int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Give me one argument!\n");
        return 0;
    }

    if(strcmp(argv[1], "handler") == 0) {
        test_handler();
    } else if(strcmp(argv[1], "ignore") == 0) {
        test_ignore();
    } else if(strcmp(argv[1], "mask") == 0) {
        test_mask();
    } else if(strcmp(argv[1], "pending") == 0) {
        test_pending();
    }
    return 0;
}