CC =  gcc -Wall -std=c11 -g


clean:
	rm -f main
zad2:
	$(CC) -o main main.c
test_sort_data:
	make clean
	make zad2
	./main data
	make clean

test_sort_nadawca:
	make clean
	make zad2
	./main nadawca
	make clean

test_send:
	make clean
	make zad2
	./main piotreto Tytul Tresc
	make clean