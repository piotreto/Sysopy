#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    pair* files1 = (pair*)calloc(1, sizeof(pair));
    files1->file1 = "text1.txt";
    files1->file2 = "text2.txt";
    files1->tmp_merged = NULL;
    pair files2;
    files2.file1 = "text3.txt";
    files2.file2 = "text4.txt";
    files2.tmp_merged = NULL;
    merge_files(files1);
    merge_files(&files2);

    main_table* tab = create_table(2);
    add_rowblock(&(tab->table[0]), 0, files1);
    add_rowblock(&(tab->table[1]),1,&files2);
    display(tab);





    return 0;
}
