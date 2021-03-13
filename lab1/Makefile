

CC =  gcc -Wall -std=c11 -g


libstatic: 
	$(CC) -c lib.c -o lib.o
	ar rcs lib.a lib.o

clean:
	rm -f *.a
	rm -f *.o
	rm -f *.out
	rm -f *test*

all: clean libstatic
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main

time_test: all
	seq 1 20 > test1.txt
	seq 1 400 > test2.txt
	seq 1 1000 > test3.txt
	./main test test1.txt test2.txt test3.txt > raport2.txt




