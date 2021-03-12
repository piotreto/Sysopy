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


typedef struct table{
    char** row;
    pair* files;
    int size;
} table;

typedef struct main_table{
    table* table;
    int size;
} main_table;

main_table* create_table(int size);
void merge_files(pair* files);
int add_rowblock(table* tab, int idx, pair* files); 
int count_lines(char* file1);
void display(main_table* tab);
void clean(main_table* tab);


#endif
