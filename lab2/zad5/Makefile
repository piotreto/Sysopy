CC =  gcc -Wall -std=c11 -g


clean:
	rm -f *.out
	rm -f *.o
	rm -f main_a
	rm -f main_b
	rm -f *.a

zad5_a:
	$(CC)  main_a.c -o main_a -lm
test1: clean 
	make zad5_a
	./main_a test_in.txt test_out.txt
	make clean
zad5_b:
	$(CC)  main_b.c -o main_b -lm
test2: clean 
	make zad5_b
	./main_b test_in.txt test_out.txt
	make clean
test_time:
	make clean
	make zad5_a
	make zad5_b
	echo "LIBRARY TEST\n" > pomiar_zad_5.txt
	./main_a test >> pomiar_zad_5.txt
	echo "\nSYSTEM TEST\n" >> pomiar_zad_5.txt
	./main_b test >> pomiar_zad_5.txt
	make clean