CC =  gcc -Wall -std=c11 -g


clean:
	rm -f *.out
	rm -f *.o
	rm -f main_a
	rm -f main_b
	rm -f *.a

zad4_a:
	$(CC)  main_a.c -o main_a -lm
test1: clean 
	make zad4_a
	./main_a test_in.txt test_out.txt ala ewa
	make clean
zad4_b:
	$(CC)  main_b.c -o main_b -lm
test2: clean 
	make zad4_b
	./main_b test_in.txt test_out.txt ala ewa
	make clean