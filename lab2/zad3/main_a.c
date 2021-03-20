#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int get_tens(int number) {
    return (number % 100 ) / 10;
}
int is_perfect(int number) {
    double sqr1 = sqrt((double)number);
    int sqr2 = sqr1;
    if(sqr1 == sqr2) return 1;
    else return -1;
}
int convert_to_int(char* string) {
    int size = strlen(string) - 1; // -1 because we dont want to count \n
    int number = 0;
    for(int i = 0;i < size;i++) {
        number = number * 10 + (string[i] - '0');
    }
    return number;

}

int is_line(char *string, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (string[i] == '\n')
        {
            return i + 1;
        }
    }
    return -1;
}
char *getline_f(FILE *fptr)
{
    size_t tmp_size = 256;
    char *tmp_string = (char *)calloc(tmp_size, sizeof(char));
    size_t readed = fread(tmp_string, sizeof(char), tmp_size, fptr);
    int count = is_line(tmp_string, readed);
    
    while(count == -1 && readed == tmp_size) { // if readed < tmp_size it means we got end of file, MonkaS
        free(tmp_string);
        fseek(fptr, -readed ,SEEK_CUR);
        tmp_size *= 2;
 
        tmp_string = (char*)calloc(tmp_size, sizeof(char));
        readed = fread(tmp_string, sizeof(char), tmp_size, fptr);
        count = is_line(tmp_string, readed);
    }
    
    
    char *result;
    if (count == -1) // we are on last line, that doesnt contain \n
    {
        result = (char *)calloc(readed + 1, sizeof(char));
        strncpy(result, tmp_string, readed);
        result[readed] = '\0';
    }
    else
    {
        result = (char *)calloc(count + 1, sizeof(char));
        strncpy(result, tmp_string, count);
        result[count] = '\0';
        fseek(fptr, -(readed - count), SEEK_CUR);
    }
    free(tmp_string);
    return result;
}
int main(int argc, char **argv) {
    FILE* fptr = fopen("dane.txt", "r");
    if(!fptr){
      printf("You have to create a file \"dane.txt\""); 
      return 0;
    }
    FILE* out1 = fopen("a.txt", "w");
    FILE* out2 = fopen("b.txt", "w");
    FILE* out3 = fopen("c.txt", "w");

    int counter = 0;

    char* line = NULL;
    while(feof(fptr) == 0) {
        line = getline_f(fptr);
        int number = convert_to_int(line);
        if(number % 2 == 0) counter += 1;
        int tens = get_tens(number);
        if(tens == 0 || tens == 7) {
            fwrite(line, sizeof(char), strlen(line), out2);
        }
        if(is_perfect(number) == 1) {
            fwrite(line, sizeof(char), strlen(line), out3);
        }
    }
    char result[256] = "Liczb parzystych jest ";
    char number_str[10];
    sprintf(number_str, "%d\n", counter);
    strcat(result, number_str);
    fwrite(result, sizeof(char), strlen(result), out1);
    fclose(fptr);
    fclose(out1);
    fclose(out2);
    fclose(out3);

    return 0;
}
