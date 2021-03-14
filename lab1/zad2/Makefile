

CC =  gcc -Wall -std=c11 -g


libstatic: 
	$(CC) -c lib.c -o lib.o
	ar rcs lib.a lib.o

clean:
	rm -f *.a
	rm -f *.o
	rm -f *.out
	rm -f *test*
	rm -f main

run: clean libstatic
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main
	./main create_table 2 merge_files text1.txt:text2.txt text3.txt:text4.txt remove_block 0 remove_row 1 1
	make clean

test: clean libstatic
	seq 1 20 > test1.txt
	seq 1 400 > test2.txt
	seq 1 1000 > test3.txt
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main
	./main test test1.txt test2.txt test3.txt > raport2.txt
	make clean




