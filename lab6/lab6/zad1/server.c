#include "common.h"


pid_t users[MAX_USERS];
int q_users[MAX_USERS];
int c_users[MAX_USERS];
int count;
int main_q;

void handle_sigint() {
    exit(0);
}

void close_everything() {
    for(int i = 0;i < count;i++) {
        kill(users[i], SIGINT);
    }
    msgctl(main_q, IPC_RMID, NULL);
}
int get_free_idx() {
    int i = 0;
    while(i < count && users[i] != 0) {
        i += 1;
    }
    return i;
}
void init_client(message *msg) {
    int idx = get_free_idx();
    if(idx == count) count += 1;
    users[idx] = msg->sender_pid;
    q_users[idx] = atoi(msg->message_str);
    
    

    message init_msg;
    init_msg.mtype = INIT;
    init_msg.sender_pid = getpid();
    sprintf(init_msg.message_str, "%d", idx);
    msgsnd(q_users[idx], &init_msg, size, 0);

}

void list(message *msg) {
    printf("Avaible users: \n");
    for(int i =0; i < count; i++){
        if(users[i] != 0)
            printf("ID: %d, busy?: %d \n", i, c_users[i]);
    }
    return;
}

void disconnect(message *msg) {
    int i = 0;
    while(users[i] != msg->sender_pid){
        i += 1;
    }
    c_users[i] = 0;

}

void stop(message *msg) {
    int i = 0;
    while(users[i] != msg->sender_pid){
        i += 1;
    }
    users[i] = 0;
    q_users[i] = 0;
    c_users[i] = 0;
}

void connect(message *msg) {

    int connect_to = atoi(msg->message_str);

    message info;
    info.mtype = CONNECT;
    info.sender_pid = getpid();

    int i = 0;
    while(users[i] != msg->sender_pid){
        i += 1;
    }

    if(connect_to >= count) {
        sprintf(info.message_str, "%d %d", -1, users[connect_to]);
    } else {
        sprintf(info.message_str, "%d %d", q_users[connect_to], users[connect_to]);
        c_users[i] = 1;
        c_users[connect_to] = 1;
    }

    msgsnd(q_users[i], &info, size, 0);
    

}
void process_message(message* msg) {
    if(msg->mtype == INIT) {
        init_client(msg);
        return;
    }
    if(msg->mtype == LIST) {
        list(msg);
        return;
    }
    if(msg->mtype == DISCONNECT) {
        disconnect(msg);
        return;
    }
    if(msg->mtype == STOP) {
        stop(msg);
        return;
    }
    if(msg->mtype == CONNECT) {
        connect(msg);
        return;
    }
}



int main() {
    atexit(close_everything);
    signal(SIGINT, handle_sigint);
    char* path = getenv("HOME");

    main_q = create_q(path, PROJECT_ID);

    message msg;
    while(1){
        if(msgrcv(main_q, &msg, size, -INIT-1, 0) == -1){
            printf("server: Error receiving message");
            exit(1);
        }

        process_message(&msg);
    }
}