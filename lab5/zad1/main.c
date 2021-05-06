#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define NAME_N 64
#define CMD_N 64
#define ARGS_N 5
#define LINE_N 256
#define K_MAX 10
#define CMD 4


struct cmd_line {
    char name[NAME_N];
    char line[LINE_N];
};

struct cmd_info {
    char* args[ARGS_N];
    int argc;
};

int skip_to_next_space(char* buff) {
    int i = 0;
    while(buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\0') {
        i++;
    }
    return i;
}

int parse_definitions(FILE* fptr, struct cmd_line* commands){
    char line[LINE_N];
    int idx = 0;
    int step = 0;
    do {
        fgets(line, LINE_N, fptr);
        if(line[0] == '\n') {
            continue;
        }
        step = skip_to_next_space(line);
        strncpy(commands[idx].name, line,step); // getting "skladnikx"
        commands[idx].name[step] = '\0';
        strncpy(commands[idx].line, line + step + 3, LINE_N);

        commands[idx].line[strlen(commands[idx].line) - 1] = '\0';

        idx += 1;
    }while(line[0] != '\n');

    return idx;

}

int getCommands(char* line, struct cmd_info* commands){
    int idx = 0;
    int argc = 0;

    char* token = strtok(line, " ");
    while(token != NULL){
        if(token[0] == '|'){
            commands[idx].args[argc] = NULL;
            commands[idx].argc = argc + 1;
            argc = 0;
            idx += 1;
        } else {
            commands[idx].args[argc] = (char*)calloc(LINE_N, sizeof(char));
            strcpy(commands[idx].args[argc], token);
            argc += 1;
        }
        token = strtok(NULL, " ");
    }
    commands[idx].args[argc] = NULL;
    commands[idx].argc = argc + 1;

    return idx+1;
}

void close_pipes(int (*pipes)[2], int n){
    for(int i = 0; i < n; i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}


int main(int argc, char**argv) {
    FILE* fptr;
    fptr = fopen("commands.txt", "r");

    if (fptr == NULL) {
        printf("I couldnt open file with commands\n");
        return 1;
    } else {
        struct cmd_line commands_definitions[K_MAX];
        int size_d = parse_definitions(fptr, commands_definitions);
        struct cmd_info commands[CMD];

        int pipes[CMD][2];
        char* line_buf = NULL;
        size_t line_buf_size = 0;
        while(getline(&line_buf, &line_buf_size, fptr) > 0) {
            char parsed_line[LINE_N];
            parsed_line[0] = '\0';
            if(line_buf[strlen(line_buf) - 1] == '\n'){
                line_buf[strlen(line_buf) - 1] = '\0';
            }
            char* token = strtok(line_buf, " ");
            while(token != NULL) {
                if (token[0] != '|') {
                    int idx = atoi(&token[strlen(token) - 1]);
                    strcat(parsed_line, commands_definitions[idx-1].line);
                } else {
                    strcat(parsed_line, " | ");
                }
                token = strtok(NULL, " ");
            }

            int size_c = getCommands(parsed_line, commands);

            for(int i = 0;i < size_c;i++) {
                pipe(pipes[i]);
            }
            
            for(int i = 0;i < size_c;i++) {
                int pid = fork();
                if (pid == 0) {
                    if(i != 0) {
                        dup2(pipes[i-1][0], STDIN_FILENO); // ustawiam IN
                    }
                    if(i != size_c-1){
                        dup2(pipes[i][1], STDOUT_FILENO);
                    }
                    close_pipes(pipes, size_c);
                    if (execvp(commands[i].args[0], commands[i].args) == -1) {
                        printf("%s", commands[i].args[0]);
                        printf("EEEERRROOOR\n");
                    };
                    
                    exit(0);
                }
            }
            close_pipes(pipes, size_c);
            while(wait(NULL) > 0);

        }
        free(line_buf);
        fclose(fptr);
    }
}