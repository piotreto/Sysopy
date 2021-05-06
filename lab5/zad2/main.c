#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


void sort_mails(char* option) {
    char* command;
    if(strcmp(option, "nadawca") == 0) {
        command = "echo  | mail | sort -k 3";
    } else {
        command = "echo | mail -H";
    }
    FILE* mail_input = popen(command, "r");

    char* buff = NULL;
    size_t buff_size = 0;

    while(getline(&buff, &buff_size, mail_input) > 0) {
        printf("%s", buff);
    }
    free(buff);
    pclose(mail_input);
}

void send_mail(char* adress, char* title, char* txt) {
    char* command = (char*)calloc(strlen(adress) + strlen(title) + 30, sizeof(char));
    sprintf(command, "mail -s %s %s", title, adress);

    FILE* mail_input = popen(command, "w");

    fputs(txt, mail_input);

    pclose(mail_input);

}


int main(int argc, char ** argv) {
    if(argc == 2) {
        sort_mails(argv[1]);
    } else if(argc == 4) {
        send_mail(argv[1], argv[2], argv[3]);
    } else {
        printf("Bledna liczba argumentow.\n");
    }
    return 0;
}