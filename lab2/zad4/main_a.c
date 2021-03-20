#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



void change_pattern(char* file1, char* file2, char* n1, char* n2) {
    FILE* fptr1 = fopen(file1,"r");
    FILE* fptr2 = fopen(file2, "w");
    if(fptr1 && fptr2) {
        char ch;
        while(fread(&ch, sizeof(char), 1, fptr1) != 0) {
            if(ch == n1[0]) {
                int len = 1;
                char temp;
                while(feof(fptr1) == 0 && len < strlen(n1)){
                    fread(&temp, sizeof(char), 1, fptr1);
                    if(temp == n1[len]){
                        len += 1;
                    } else {
                        break;
                    }
                }
                if(len == strlen(n1)) {
                    fwrite(n2, sizeof(char), len, fptr2);
                } else {
                    fwrite(&ch, sizeof(char), 1, fptr2);
                    fseek(fptr1, -(len), SEEK_CUR);
                }
            }
            else {
                fwrite(&ch, sizeof(char), 1, fptr2);
            }
        }
    }else {
        printf("Opening files failed\n");
        return;
    }
    fclose(fptr1);
    fclose(fptr2);

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