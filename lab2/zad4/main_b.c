#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>



void change_pattern(char* file1, char* file2, char* n1, char* n2) {
    int fptr1 = open(file1,O_RDONLY);
    int fptr2 = open(file2, O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    if(fptr1 != -1 && fptr2 != -1) {
        char ch;
        while(read(fptr1, &ch, 1) != 0) {
            if(ch == n1[0]) {
                int len = 1;
                char temp;
                while(len < strlen(n1) && read(fptr1, &temp, 1) != 0){
                    if(temp == n1[len]){
                        len += 1;
                    } else {
                        break;
                    }
                }
                if(len == strlen(n1)) {
                    write(fptr2, n2, len);
                } else {
                    write(fptr2, &ch, 1);
                    lseek(fptr1, -(len), SEEK_CUR);
                }
            }
            else {
                write(fptr2, &ch, 1);
            }
        }
    }else {
        printf("Opening files failed\n");
        return;
    }
    close(fptr1);
    close(fptr2);

    return;
}


int main(int argc, char **argv) {
    if(argc < 5) {
        printf("You have to give me 5 arguments to start!!!!\n");
        return 0;
    }
    char* file1 = argv[1];
    char* file2 = argv[2];
    char* n1 = argv[3];
    char* n2 = argv[4];
    change_pattern(file1, file2, n1, n2);

    return 0;

}