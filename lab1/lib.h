#ifndef _NAGLOWEK_H
#define _NAGLOWEK_H
#include <stdio.h>




typedef struct pair
{
    char* file1;
    char* file2;
    FILE* tmp_merged;
    int size;
}pair;

typedef struct block{
    char** row;
    pair* files;
    int size;
} block;

typedef struct main_table{
    block** block_table;
    int size;
} main_table;

main_table* create_table(int size);
pair* merge_sequence(main_table* tab, char** argv);
void merge_files(pair* files);
block* add_rowblock(int idx, pair* files);
void delete_block(main_table* tab, int block_index);
void delete_row(main_table* tab, int block_index, int row_index);




int count_lines(char* file1);
void display(main_table* tab);
void clean(main_table* tab);


#endif
