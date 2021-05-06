#include "common.h"

mqd_t server_q;
mqd_t connected_q;
pid_t connected_pid;
mqd_t my_q;
int my_id;
char path[256];

// void info_to_connected() {
//     if (connected_pid == 0) {
//         EXIT("client: Invalid pid")
//     }
//     kill(connected_pid, SIGUSR1);
// }

void send_disconnect_to_server() {
    message dism;
    dism.sender_pid = getpid();

    if (mq_send(server_q,(char*) &dism, size, DISCONNECT) == -1) {
        printf("Error while sending disconnect to server\n");
        exit(1);
    }
    connected_pid = 0;
    connected_q = 0;
    printf("\n--server--\n");
}

void send_disconnect() {
    if (connected_q != 0) {
        message m;
        m.sender_pid = getpid();
        mq_send(connected_q,(char*) &m, size, DISCONNECT);
        mq_close(connected_q);
        send_disconnect_to_server();
    }
}

void send_message(char *m_str) {
    message to_client;
    to_client.sender_pid = getpid();
    strcpy(to_client.message_str, m_str);

    if (mq_send(connected_q, (char*) &to_client, size, CONNECT) == -1) {
        printf("Error while sending message to client.\n");
        exit(1);
    }
}

void close_and_disconnect() {
    send_disconnect();

    message m;
    
    m.sender_pid = getpid();
    mq_send(server_q, &m, size, STOP);
    mq_close(my_q);
    mq_unlink(path);
}

void send_connect(int com_id) {
    if (com_id == my_id) {
        printf("Can't connect to yourself\n");
        return;
    }

    message m;
    m.sender_pid = getpid();
    sprintf(m.message_str, "%d", com_id);

    if (mq_send(server_q, (char*)&m, size, CONNECT) == -1) {
        printf("Error while sending message to server.\n");
        exit(1);
    }

    message from_server;

    if (mq_receive(my_q, (char*)&from_server, size, NULL) == -1) {
        printf("Error while receiving messega from server\n");
        exit(1);
    }

    sscanf(from_server.message_str, "%d", &connected_pid);


    if (connected_pid == 0) {
        printf("Error while connecting to client\n");
        exit(1);
    }
    char* connected_path[256];
    sprintf(connected_path, "/%d", connected_pid);

    connected_q = open_q(connected_path);


    message to_client;
    to_client.sender_pid = getpid();
    sprintf(to_client.message_str, "%d", getpid());

    if (mq_send(connected_q, (char*)&to_client, size, CONNECT) == -1) {
        printf("Error while sending message to client\n");
        exit(1);
    }

    printf("\n========chat========\n");
    struct sigevent callback;
    callback.sigev_notify = SIGEV_SIGNAL;
    callback.sigev_signo = SIGUSR1;

    mq_notify(my_q, &callback);
}

void send_list() {
    message to_server;
    to_server.sender_pid = getpid();

    if (mq_send(server_q, &to_server, size, LIST) == -1) {
        printf("Error while sending LIST command\n");
        exit(1);
    }
}

void receive_notification() {
    message m;
    unsigned int mtype;
    if (mq_receive(my_q, (char*) &m, size, &mtype) == -1) {
        printf("Error while receiving message from server\n");
        exit(1);
    }

    if (mtype == CONNECT) {
        if(connected_q == 0){
            connected_pid = m.sender_pid;
            char connected_path[10];
            sprintf(connected_path, "/%d", connected_pid);
            connected_q = open_q(connected_path);
            sscanf(m.message_str, "%d", &connected_q);
            printf("\n========chat========\n");
        } else {
            printf("%s\n", m.message_str);
            fflush(stdout);
        }
        struct sigevent callback;
        callback.sigev_notify = SIGEV_SIGNAL;
        callback.sigev_signo = SIGUSR1;
        mq_notify(my_q, &callback);
        
    } else if (mtype == DISCONNECT) {
        send_disconnect_to_server();
    }
}

void handle_sigint() { exit(1); }

void send_init(){
    message to_client;
    to_client.sender_pid = getpid();

    if (mq_send(server_q, (char*) &to_client, size, INIT) == -1) {
        printf("Error while initing client\n");
        exit(1);
    }
    message from_server;

    if (mq_receive(my_q, &from_server, size, NULL) == -1) {
        printf("Error while receiving message from server\n");
        exit(1);
    }

    sscanf(from_server.message_str, "%d", &my_id);
    printf("client: Your ID is %d\n", my_id);
    fflush(stdout);
}

int main() {
    atexit(close_and_disconnect);

    signal(SIGINT, handle_sigint);

    signal(SIGUSR1, receive_notification);

    sprintf((char*) path, "/%d", getpid());


    server_q = open_q(SERVER_Q);

    my_q = create_q(path);

    send_init();

    char in[MAX_MSG_LEN];

    struct sigevent callback;
    callback.sigev_notify = SIGEV_SIGNAL;
    callback.sigev_signo = SIGUSR1;

    mq_notify(my_q, &callback);

    while (1) {
        fgets(in, MAX_MSG_LEN, stdin);
        in[strlen(in) - 1] = '\0';

        if (strcmp(in, "DISCONNECT") == 0) {
            send_disconnect();
            continue;
        }

        if (connected_q != 0) {
            send_message(in);
            continue;
        }

        if (strcmp(in,  "LIST") == 0) {
            send_list();
        } else if (strcmp(in, "STOP") == 0) {
            exit(0);
        } else {
            char command[MAX_MSG_LEN];
            int com_id;
            sscanf(in, "%s %d", command, &com_id);

            if (strcmp(command, "CONNECT") == 0) {
                send_connect(com_id);
            } else {
                printf("Unknown command %s\n", command);
                fflush(stdout);
            }
        }
    }

    return 0;
}