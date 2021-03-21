CC =  gcc -Wall -std=c11 -g


clean:
	rm -f *.out
	rm -f *.o
	rm -f main_a
	rm -f main_b
	rm -f *.a
zad2_a:
	$(CC) -o main_a main_a.c
test1: clean
	make zad2_a
	./main_a test1.txt c
	make clean
zad2_b:
	$(CC) -o main_b main_b.c
test2: clean
	make zad2_b
	./main_b test1.txt c
	make clean
test_time:
	make clean
	make zad2_a
	make zad2_b
	echo "LIBRARY TEST\n" > pomiar_zad_2.txt
	./main_a test >> pomiar_zad_2.txt
	echo "\nSYSTEM TEST\n" >> pomiar_zad_2.txt
	./main_b test >> pomiar_zad_2.txt
	make clean
