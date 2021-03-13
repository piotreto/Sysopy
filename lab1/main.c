#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>





typedef struct time_struct {
    double real;
    double user;
    double sys;
} time_struct;


void time_test(char* file, int n) {

    struct tms test_times[5]; //usr and sys
    clock_t test_times_real[5];

    main_table* tab = create_table(n);
    pair* seq = (pair*)calloc(n, sizeof(pair));



    // merging elements
    times(&test_times[0]);
    test_times_real[0] = clock();

    for(int i = 0;i < n;i++){
        seq[i].file1 = file;
        seq[i].file2 = file;
        merge_files(seq + i);
    }


    //saving block_rows
    times(&test_times[1]);
    test_times_real[1] = clock();

    for(int i = 0;i < n;i++) {
        tab->block_table[i] = add_rowblock(i, &seq[i]);
    }

    //deleting blocks
    times(&test_times[2]);
    test_times_real[2] = clock();

    for(int i = 0;i < n;i++) {
        delete_block(tab, i);
    }

    //adding and deleting
    times(&test_times[3]);
    test_times_real[3] = clock();

    clean(tab);

    tab = create_table(n);

    for(int i = 0;i < n;i++) {
        tab->block_table[i] = add_rowblock(i, &seq[i]);
        delete_block(tab, i);
    }

    times(&test_times[4]);
    test_times_real[4] = clock();

    clean(tab);
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

    }
    if(strcmp(argv[1], "create_table") != 0){
        puts("Firstly you have to create table");
        
    }

    main_table* tab = create_table(2);
    pair* seq = merge_sequence(tab, argv+1);
    tab->block_table[0] = add_rowblock(0, &seq[0]);
    tab->block_table[1] = add_rowblock(1, &seq[1]);
    display(tab);
    delete_row(tab, 0,2);
    display(tab);
    delete_block(tab, 0);
    display(tab);
    clean(tab);
    free(seq);

    return 0;
}
