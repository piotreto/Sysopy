#define  _GNU_SOURCE
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))


main_table* create_table(int size) {
    main_table* result = (main_table*)calloc(1,sizeof(main_table));
    result->table = (table*)calloc(size, sizeof(table));
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

int add_rowblock(table* tab, int idx, pair* files) {
    tab->size = files->size;
    tab->files = files;
    tab->row = (char**)calloc(tab->size, sizeof(char*));
    size_t size_buf = 0;
    for(int i = 0;i < tab->size;i++) {
        getline(&(tab->row[i]), &size_buf, files->tmp_merged);
    }
    return idx;
}

void display(main_table* tab) {
    for(int i = 0;i < tab->size;i++){
        for(int j = 0;j < tab->table[i].size;j++) {
            printf("%s", (tab->table[i]).row[j]);
        }
    }
}