#include "common.h"

int server_q;
int connected_q;
pid_t connected_pid;
int my_q;
int my_id;

void info_to_connected() {
    if (connected_pid == 0) {
        printf("Invalid pid!\n");
        exit(1);
    }
    kill(connected_pid, SIGUSR1);
}

void send_disconnect_to_server() {
    message dism;
    dism.mtype = DISCONNECT;
    dism.sender_pid = getpid();

    if (msgsnd(server_q, &dism, size, 0) == -1) {
        printf("Error while sending DISCONNECT messege to server\n");
        exit(1);
    }
    connected_pid = 0;
    connected_q = 0;
    printf("\n--server--\n");
}

void send_disconnect() {
    if (connected_q != 0) {
        message m;
        m.mtype = DISCONNECT;
        m.sender_pid = getpid();
        if(msgsnd(connected_q, &m, size, 0) == -1) {
            printf("Error while sending DISCONNECT command to connected ID\n");
            exit(1);
        }
        info_to_connected();
        send_disconnect_to_server();
    }
}

void send_message(char *m_str) {
    message to_client;
    to_client.mtype = CONNECT;
    to_client.sender_pid = getpid();
    strcpy(to_client.message_str, m_str);

    if (msgsnd(connected_q, &to_client, size, 0) == -1) {
        printf("Error while chatting with connected ID\n");
        exit(1);
    }
    info_to_connected();
}

void close_and_disconnect() {
    send_disconnect();

    message m;
    m.mtype = STOP;
    m.sender_pid = getpid();
    msgsnd(server_q, &m, size, 0);

    msgctl(my_q, IPC_RMID, NULL);
}

void send_connect(int com_id) {
    if (com_id == my_id) {
        printf("Can't connect to yourself\n");
        return;
    }

    message m;
    m.mtype = CONNECT;
    m.sender_pid = getpid();
    sprintf(m.message_str, "%d", com_id);

    if (msgsnd(server_q, &m, size, 0) == -1) {
        printf("Error while sending message  to server\n");
        exit(1);
    }

    message from_server;

    if (msgrcv(my_q, &from_server, size, CONNECT, 0) == -1) {
        printf("Error while receiving message.\n");
        exit(1);
    }

    sscanf(from_server.message_str, "%d %d", &connected_q, &connected_pid);

    if (connected_q == -1 || connected_pid == 0) {
        printf("Error while connecting to client\n");
        exit(1);
    }

    message to_client;
    to_client.mtype = CONNECT;
    to_client.sender_pid = getpid();
    sprintf(to_client.message_str, "%d", my_q);

    if (msgsnd(connected_q, &to_client, size, 0) == -1) {
        printf("Error while sending message  to connected ID\n");
        exit(1);
    }

    printf("\n========chat========\n");
    info_to_connected();
}

void send_list() {
    message to_server;
    to_server.mtype = LIST;
    to_server.sender_pid = getpid();

    if (msgsnd(server_q, &to_server, size, 0) == -1) {
        printf("Error while sending message to server\n");
        exit(1);
    }
}

void receive_notification() {
    message m;
    if (msgrcv(my_q, &m, size, -INIT - 1, 0) == -1) {
        printf("Error while receiving message from server");
        exit(1);
    }

    if (m.mtype == CONNECT) {
        if(connected_q == 0){
            sscanf(m.message_str, "%d", &connected_q);
            connected_pid = m.sender_pid;
            printf("\n========chat========\n");
            return;
        }
        printf("%s\n", m.message_str);
        fflush(stdout);
    } else if (m.mtype == DISCONNECT) {
        send_disconnect_to_server();
    }
}

void handle_sigint() { exit(1); }

void send_init(){
    message to_client;
    to_client.mtype = INIT;
    to_client.sender_pid = getpid();
    sprintf(to_client.message_str, "%d", my_q);

    if (msgsnd(server_q, &to_client, size, 0) == -1) {
        printf("Error while sending message to server\n");
        exit(1);
    }

    message from_server;

    if (msgrcv(my_q, &from_server, size, INIT, 0) == -1) {
        printf("Error while receiving message from server");
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

    char *path = getenv("HOME");

    key_t k = ftok(path, PROJECT_ID);

    server_q = msgget(k, 0);

    my_q = create_q(path, (char) (getpid() % 'a'));

    send_init();

    char in[MAX_MSG_LEN];

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