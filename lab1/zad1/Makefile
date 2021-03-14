
CC =  gcc -Wall -std=c11 -g


libstatic: 
	$(CC) -c lib.c -o lib.o
	ar rcs lib.a lib.o

libshared:
	$(CC) -fPIC -shared lib.c -o lib_shared.so