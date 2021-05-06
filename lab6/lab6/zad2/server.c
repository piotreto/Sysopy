#include "common.h"


pid_t users[MAX_USERS];
mqd_t q_users[MAX_USERS];
int c_users[MAX_USERS];
int count;
mqd_t main_q;

void handle_sigint() {
    exit(0);
}

void close_everything() {
    for(int i = 0;i < count;i++) {
        mq_close(q_users[i]);
        kill(users[i], SIGINT);
    }
    mq_unlink(SERVER_Q);
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
    char path[10];
    sprintf(path, "/%d", users[idx]);
    q_users[idx] = open_q(path);
    
    

    message init_msg;
    init_msg.sender_pid = getpid();
    sprintf(init_msg.message_str, "%d", idx);

    if(mq_send(q_users[idx], (char*) &init_msg, size, INIT) == -1){
        printf("Error while sending INIT message from server.\n");
        exit(1);
    }


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
    info.sender_pid = getpid();

    int i = 0;
    while(users[i] != msg->sender_pid){
        i += 1;
    }

    if(connect_to >= count) {
        sprintf(info.message_str, "%d", 0);
    } else {
        sprintf(info.message_str, "%d", users[connect_to]);
        c_users[i] = 1;
        c_users[connect_to] = 1;
    }

    if (mq_send(q_users[i], (char*) &info, size, CONNECT) == -1) {
        printf("Error while sending CONNECT message from server\n");
        exit(1);
    }
    

}
void process_message(message* msg, unsigned int mtype) {
    if(mtype == INIT) {
        init_client(msg);
        return;
    }
    if(mtype == LIST) {
        list(msg);
        return;
    }
    if(mtype == DISCONNECT) {
        disconnect(msg);
        return;
    }
    if(mtype == STOP) {
        stop(msg);
        return;
    }
    if(mtype == CONNECT) {
        connect(msg);
        return;
    }
}



int main() {
    atexit(close_everything);
    signal(SIGINT, handle_sigint);

    main_q = create_q(SERVER_Q);

    message msg;
    unsigned int mtype;
    while(1){
        if(mq_receive(main_q, (char *) &msg, size, &mtype) == -1){
            printf("Error while receiving messgae\n");
            exit(1);
        }

        process_message(&msg, mtype);
    }

    return 0;
}