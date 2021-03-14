#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>



void* dl_handle;


block* (*dl_add_rowblock) (int idx, pair* files);
void (*dl_delete_block) (main_table* tab, int block_index);
void (*dl_delete_row) (main_table* tab, int block_index, int row_index);
main_table* (*dl_create_table)(int size);
pair* (*dl_merge_sequence) (main_table* tab, char** argv);
void (*dl_display)(main_table* tab);
void (*dl_clean)(main_table* tab);
void (*dl_merge_files)(pair* files);








typedef struct time_struct {
    double real;
    double user;
    double sys;
} time_struct;


void time_test(char* file, int n) {

    struct tms test_times[5]; //usr and sys
    clock_t test_times_real[5];

    main_table* tab = dl_create_table(n);
    pair* seq = (pair*)calloc(n, sizeof(pair));



    // merging elements
    times(&test_times[0]);
    test_times_real[0] = clock();

    for(int i = 0;i < n;i++){
        seq[i].file1 = file;
        seq[i].file2 = file;
        dl_merge_files(seq + i);
    }


    //saving block_rows
    times(&test_times[1]);
    test_times_real[1] = clock();

    for(int i = 0;i < n;i++) {
        tab->block_table[i] = dl_add_rowblock(i, &seq[i]);
    }

    //deleting blocks
    times(&test_times[2]);
    test_times_real[2] = clock();

    for(int i = 0;i < n;i++) {
        dl_delete_block(tab, i);
    }

    //adding and deleting
    times(&test_times[3]);
    test_times_real[3] = clock();

    dl_clean(tab);

    tab = dl_create_table(n);

    for(int i = 0;i < n;i++) {
        tab->block_table[i] = dl_add_rowblock(i, &seq[i]);
        dl_delete_block(tab, i);
    }

    times(&test_times[4]);
    test_times_real[4] = clock();

    dl_clean(tab);
    free(seq);

    time_struct time_tab[4];

    for(int i = 0;i < 4;i++){
        time_tab[i].real = (double) (test_times_real[i+1] - test_times_real[i]) / CLOCKS_PER_SEC;
        time_tab[i].sys = (double) (test_times[i+1].tms_stime - test_times[i].tms_stime) / sysconf(_SC_CLK_TCK);
        time_tab[i].user = (double) (test_times[i+1].tms_utime - test_times[i].tms_utime) / sysconf(_SC_CLK_TCK);
    }


    printf("\nMERGE\n");
    printf("REAL         USER        SYSTEM\n");
    printf("%lfs    %lfs   %lfs\n", time_tab[0].real, time_tab[0].user, time_tab[0].sys);
    
    printf("\nSAVING\n");
    printf("REAL         USER        SYSTEM\n");
    printf("%lfs    %lfs   %lfs\n", time_tab[1].real, time_tab[1].user, time_tab[1].sys);

    printf("\nDELETING\n");
    printf("REAL         USER        SYSTEM\n");
    printf("%lfs    %lfs   %lfs\n", time_tab[2].real, time_tab[2].user, time_tab[2].sys);

    printf("\nSAVE AND DELATE\n");
    printf("REAL         USER        SYSTEM\n");
    printf("%lfs    %lfs   %lfs\n", time_tab[3].real, time_tab[3].user, time_tab[3].sys);

}

int main(int argc, char **argv) {

    if(argc <= 1) {
        return 0;
    }
    dl_handle = dlopen("./libmycode.so", RTLD_LAZY);
    if(dl_handle == NULL) {
        printf("I couldnt find a library..");
        dlclose(dl_handle);
        return 0;
    }

    dl_add_rowblock = dlsym(dl_handle,"add_rowblock");
    dl_delete_block = dlsym(dl_handle,"delete_block");
    dl_delete_row = dlsym(dl_handle,"delete_row");
    dl_create_table = dlsym(dl_handle,"create_table");
    dl_merge_sequence = dlsym(dl_handle,"merge_sequence");
    dl_display = dlsym(dl_handle,"display");
    dl_clean = dlsym(dl_handle,"clean");
    dl_merge_files = dlsym(dl_handle, "merge_files");


    if(strcmp(argv[1], "test") == 0) {

        puts("-----TEST 1-----");
        puts("----20 row----");
        puts("");
        puts("----5 pairs----");
        time_test("test1.txt", 5);
        puts("");
        puts("----200 pairs----");
        time_test("test1.txt", 200);
        puts("");
        puts("----1000 pairs----");
        time_test("test1.txt", 1000);

        puts("");
        puts("");
        
        puts("-----TEST 2-----");
        puts("----400 rows----");
        puts("");
        puts("----5 pairs----");
        time_test("test2.txt", 5);
        puts("");
        puts("----200 pairs----");
        time_test("test2.txt", 200);
        puts("");
        puts("----1000 pairs----");
        time_test("test2.txt", 1000);

        puts("");
        puts("");

        puts("-----TEST 3-----");
        puts("----1000 rows----");
        puts("");
        puts("----5 pairs----");
        time_test("test3.txt", 5);
        puts("");
        puts("----200 pairs----");
        time_test("test3.txt", 200);
        puts("");
        puts("----1000 pairs----");
        time_test("test3.txt", 1000);
        dlclose(dl_handle);
        return 0;

    }
    if(strcmp(argv[1], "create_table") != 0){
        puts("Firstly you have to create table");
        dlclose(dl_handle);
        return 0;
    }
    int size = atoi(argv[2]);
    main_table* tab = dl_create_table(size);
    if(strcmp(argv[3], "merge_files") != 0) {
        puts("Nothing happend");
        free(tab);
    }
    pair* seq = dl_merge_sequence(tab, argv + 4);

    for(int i = 0;i < size;i++){
        tab->block_table[i] = dl_add_rowblock(i, &seq[i]);
    }

    puts("");
    puts("Your merged files, wooho!");
    puts("");
    dl_display(tab);

    for(int i = size + 4; i < argc;i++) {
        if(strcmp(argv[i], "remove_block") == 0){
            i += 1;
            int idx = atoi(argv[i]);
            dl_delete_block(tab, idx);
        } else if(strcmp(argv[i], "remove_row") == 0) {
            i += 1;
            int block_index = atoi(argv[i]);
            i += 1;
            int row_index = atoi(argv[i]);
            dl_delete_row(tab, block_index, row_index);
        } else {
            break;
        }
    }

    puts("Your final result");
    dl_display(tab);
    dl_clean(tab);
    free(seq);
    dlclose(dl_handle);

    return 0;
}
