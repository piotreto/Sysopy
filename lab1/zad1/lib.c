#define  _GNU_SOURCE
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))





main_table* create_table(int size) {
    main_table* result = (main_table*)calloc(1,sizeof(main_table));
    result->block_table = (block**)calloc(size, sizeof(block*));
    result->size = size;
    return result;
}

int count_lines(char* file1) {
    FILE *fptr1 = fopen(file1, "r");
    if(fptr1 == NULL){
        return 0;;
    }
    char buf[200];
    int lines_count = 0;
    while(fgets(buf, sizeof(buf), fptr1) != NULL && buf[0] != '\0'){
        lines_count += 1;
    }
    fclose(fptr1);
    return lines_count;
}

void merge_files(pair* files) {
    int lines_count = MIN(count_lines(files->file1), count_lines(files->file2));
    files->size = 2*lines_count;
    FILE *fptr1 = fopen(files->file1, "r");
    FILE *fptr2 = fopen(files->file2, "r");
    if(fptr1 == NULL || fptr2 == NULL || lines_count == 0){
        return;
    }
    files->tmp_merged = tmpfile();
    char buf[200];
    for(int i = 0;i < lines_count;i++){
        fgets(buf, sizeof(buf), fptr1);
        if(i == lines_count - 1)
            fprintf(files->tmp_merged, "%s\n", buf);
        else
            fprintf(files->tmp_merged, "%s", buf);
        fgets(buf, sizeof(buf), fptr2);
        if(i == lines_count - 1)
            fprintf(files->tmp_merged, "%s\n", buf);
        else
            fprintf(files->tmp_merged, "%s", buf);
    }

    rewind(files->tmp_merged);

    fclose(fptr1);
    fclose(fptr2);
}


block* add_rowblock(int idx, pair* files) {
    block* result = (block*)calloc(1, sizeof(block));
    result->size = files->size;
    result->files = files;
    result->row = (char**)calloc(result->size, sizeof(char*));
    size_t size_buf = 0;
    for(int i = 0;i < result->size;i++) {
        getline(&(result->row[i]), &size_buf, files->tmp_merged);
        size_buf = 0;
    }
    return result;
}


pair* merge_sequence(main_table* main_tab, char** argv){
    int size = main_tab->size;
    pair* result = (pair*)calloc(size, sizeof(pair));
    for(int i = 0; i < size; i++){
        result[i].file1 = strtok(argv[i], ":");
        result[i].file2 = strtok(NULL, "");
        merge_files(result + i);
    }
    return result;
}

void delete_block(main_table* main_tab, int block_index) {
    free(main_tab->block_table[block_index]->row);
    main_tab->block_table[block_index]->row = NULL;
    free(main_tab->block_table[block_index]);
    main_tab->block_table[block_index] = NULL;
}

void delete_row(main_table* main_tab, int block_index, int row_index) {
    block* block = main_tab->block_table[block_index];
    if(block == NULL) return;
    if(block == NULL) return;
    free(block->row[row_index]);
    block->row[row_index] = NULL;
}

int count_rows(main_table* main_tab, int index) {
    block* block = main_tab->block_table[index];
    if(block == NULL) return 0;
    int result = 0;
    for(int i = 0;i < block->size;i++) {
        if(block->row[i] != NULL) result += 1;
    }
    return result;
}

void display(main_table* tab) {

    for(int i = 0;i < tab->size;i++){
        block* block = tab->block_table[i];
        if(block == NULL) continue;
        printf("\nNumber of rows: %d\n", count_rows(tab, i));
        for(int j = 0;j < block->size;j++) {
            if(block->row[j] == NULL) continue;
            printf("%s", block->row[j]);
        }
        puts("");
    }
}

void clean(main_table* tab) {
    for(int i = 0;i < tab->size;i++){
        if(tab->block_table[i] == NULL) continue;
        free(tab->block_table[i]->row);
        free(tab->block_table[i]);
    }
    free(tab->block_table);
    free(tab);
}