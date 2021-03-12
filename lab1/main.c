#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    main_table* tab = create_table(2);
    pair* files1 = (pair*)calloc(1, sizeof(pair));
    pair* files2 = (pair*)calloc(1, sizeof(pair));
    files1->file1 = "text1.txt";
    files1->file2 = "text2.txt";
    files2->file1 = "text3.txt";
    files2->file2 = "text4.txt";
    merge_files(files1);
    merge_files(files2);
    pair* seq = merge_sequence(tab, argv+1);
    tab->block_table[0] = add_rowblock(0, &seq[0]);
    tab->block_table[1] = add_rowblock(1, &seq[1]);
    // merge_sequence(tab, argv+1);
    display(tab);
    delete_row(tab, 0,2);
    display(tab);
    delete_block(tab, 0);
    display(tab);
    clean(tab);
    free(seq);




    return 0;
}
