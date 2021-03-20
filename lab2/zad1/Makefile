CC =  gcc -Wall -std=c11 -g


clean:
	rm -f *.out
	rm -f *.o
	rm -f main_a
	rm -f main_b
	rm -f *.a
zad1_a:
	$(CC) -o main_a main_a.c
test1: clean
	make zad1_a
	./main_a test1.txt test2.txt
	make clean
zad1_b:
	$(CC) -o main_b main_b.c
test2: clean
	make zad1_b
	./main_b test1.txt test2.txt
	make clean