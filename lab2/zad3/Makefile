CC =  gcc -Wall -std=c11 -g


clean:
	rm -f *.out
	rm -f *.o
	rm -f main_a
	rm -f main_b
	rm -f *.a
clean_test:
	rm -f a.txt
	rm -f b.txt
	rm -f c.txt
zad3_a:
	$(CC)  main_a.c -o main_a -lm
test1: clean clean_test
	make zad3_a
	./main_a
	make clean
zad3_b:
	$(CC)  main_b.c -o main_b -lm
test2: clean clean_test
	make zad3_b
	./main_b
	make clean