

CC =  gcc -Wall -std=c11 -g


libstatic: 
	$(CC) -c lib.c -o lib.o
	ar rcs lib.a lib.o

clean:
	rm -f *.a
	rm -f *.o
	rm -f *.out

all: clean libstatic
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main




