CC =  gcc -Wall -std=c11 -g


libstatic: 
	$(CC) -c lib.c -o lib.o
	ar rcs lib.a lib.o

clean:
	rm -f *.a
	rm -f *.o
	rm -f *.out
	rm -f main

run: clean libstatic
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main
	./main 2 text1.txt:text2.txt text3.txt:text4.txt
	make clean

test_time:
	seq 1 20 > test1.txt
	seq 1 400 > test2.txt
	seq 1 1000 > test3.txt

	make clean
	make libstatic
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main
	./main test > raport.txt
	make clean

