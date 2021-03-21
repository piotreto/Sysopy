#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <sys/times.h>
#include <time.h>


typedef struct time_struct {
    double real;
    double user;
    double sys;
} time_struct;


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

char *getline_s(int in)
{
    size_t tmp_size = 256;
    char *tmp_string = (char *)calloc(tmp_size, sizeof(char));
    size_t readed = read(in, tmp_string, tmp_size);
    int count = is_line(tmp_string, readed);
    while(count == -1 && readed == tmp_size) { // if readed < tmp_size it means we got end of file, MonkaS
        free(tmp_string);
        lseek(in, -readed ,SEEK_CUR);
        tmp_size *= 2;
 
        tmp_string = (char*)calloc(tmp_size, sizeof(char));
        readed = read(in, tmp_string, tmp_size);
        count = is_line(tmp_string, readed);
    }
    
    char *result;
    if (readed == 0)
    {
        result = NULL;
    }
    else if (count == -1) // we get last line in file without \n char
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
        lseek(in, -(readed - count), SEEK_CUR);
    }
    free(tmp_string);
    return result;
}
int main(int argc, char **argv) {
    struct tms test_times[2]; //usr and sys
    clock_t test_times_real[2];

    times(&test_times[0]);
    test_times_real[0] = clock();
    int fptr = open("dane.txt", O_RDONLY);
    if(fptr == -1){
      printf("You have to create a file \"dane.txt\""); 
      return 0;
    }
    int out1 = open("a.txt", O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    int out2 = open("b.txt", O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    int out3 = open("c.txt", O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    
    int counter = 0;

    char* line = NULL;
    while((line = getline_s(fptr)) != NULL) {
        int number = convert_to_int(line);
        if(number % 2 == 0) counter += 1;
        int tens = get_tens(number);
        if(tens == 0 || tens == 7) {
            write(out2, line, strlen(line));
        }
        if(is_perfect(number) == 1) {
            write(out3, line, strlen(line));
        }
    }
    char result[256] = "Liczb parzystych jest ";
    char number_str[10];
    sprintf(number_str, "%d\n", counter);
    strcat(result, number_str);
    write(out1, result, strlen(result));
    close(fptr);
    close(out1);
    close(out2);
    close(out3);
    times(&test_times[1]);
    test_times_real[1] = clock();
    time_struct time_tab;
    time_tab.real = (double) (test_times_real[1] - test_times_real[0]) / CLOCKS_PER_SEC;
    time_tab.sys = (double) (test_times[1].tms_stime - test_times[0].tms_stime) / sysconf(_SC_CLK_TCK);
    time_tab.user = (double) (test_times[1].tms_utime - test_times[0].tms_utime) / sysconf(_SC_CLK_TCK);
    printf("REAL         USER        SYSTEM\n");
    printf("%lfs    %lfs   %lfs\n", time_tab.real, time_tab.user, time_tab.sys);

    return 0;
}
