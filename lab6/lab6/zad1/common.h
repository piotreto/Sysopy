#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>
#include <errno.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>


#define MAX_MSG_LEN 256
#define MAX_USERS 10
#define PROJECT_ID 'A'

#define STOP 1
#define DISCONNECT 2
#define LIST 3
#define CONNECT 4
#define INIT 5



typedef struct {
    long mtype;
    pid_t sender_pid;
    char message_str[MAX_MSG_LEN];
} message;


int size = sizeof(message) - sizeof(long);


int create_q(const char* path, char proj_id){
    key_t p_key = ftok(path, proj_id);
    if(p_key == -1){
        printf("Error obtaining queue key\n");
        exit(1);
    }

    int q_id = msgget(p_key, IPC_CREAT | IPC_EXCL | 0666);

    if(q_id == -1){
        printf("Error creating queue\n errno: %d", errno);
        exit(1);
    }

    return q_id;
}


#endif