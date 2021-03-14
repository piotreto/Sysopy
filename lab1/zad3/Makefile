

CC =  gcc -Wall -std=c11 -g
RAPORT = raport3b.txt


libstatic: 
	$(CC) -c lib.c -o lib.o
	ar rcs lib.a lib.o

libshared:
	$(CC) -fPIC -shared lib.c -o libmycode.so

clean:
	rm -f *.a
	rm -f *.o
	rm -f *.out
	rm -f main
	rm -f *.so
	rm -f main_dynamic

clean_tmp:
	rm -f *test*

run_static: clean libstatic
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main
	./main create_table 2 merge_files text1.txt:text2.txt text3.txt:text4.txt remove_block 0 remove_row 1 1
	make clean
run_shared: clean libshared
	$(CC) main.c -L. -Wl,-rpath=. -lmycode -o main
	./main create_table 2 merge_files text1.txt:text2.txt text3.txt:text4.txt remove_block 0 remove_row 1 1
	make clean
run_dll: clean libshared
	$(CC) main_dynamic.c -ldl -o main_dynamic
	./main_dynamic create_table 2 merge_files text1.txt:text2.txt text3.txt:text4.txt remove_block 0 remove_row 1 1
	make clean

run_3a_test:
	seq 1 20 > test1.txt
	seq 1 400 > test2.txt
	seq 1 1000 > test3.txt

	echo "\nSTATIC TEST\n" > raport3a.txt
	make clean
	make libstatic
	$(CC) -c main.c -o main.o
	$(CC) -static main.o -L. lib.a -o main
	./main test test1.txt test2.txt test3.txt >> raport3a.txt
	make clean

	echo "\nSHARED TEST\n" >> raport3a.txt
	make libshared

	$(CC) main.c -L. -Wl,-rpath=. -lmycode -o main
	./main test test1.txt test2.txt test3.txt >> raport3a.txt
	make clean

	echo "\nDLL TEST\n" >> raport3a.txt
	make libshared

	$(CC) main_dynamic.c -ldl -o main_dynamic
	./main_dynamic test test1.txt test2.txt test3.txt >> raport3a.txt
	make clean
	make clean_tmp

run_3b_test:
	echo "\n OPTIMIZE -O2\n" > $(RAPORT)
	echo "\nSTATIC TEST\n" >> $(RAPORT)
	make clean
	make libstatic
	$(CC) -O2 -c main.c -o main.o
	$(CC) -O2 -static main.o -L. lib.a -o main
	./main test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean

	echo "\nSHARED TEST\n" >> $(RAPORT)
	make libshared

	$(CC) -O2 main.c -L. -Wl,-rpath=. -lmycode -o main
	./main test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean

	echo "\nDLL TEST\n" >> $(RAPORT)
	make libshared

	$(CC) -O2 main_dynamic.c -ldl -o main_dynamic
	./main_dynamic test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean
	make clean_tmp

	echo "\n OPTIMIZE -O0\n" >> $(RAPORT)
	echo "\nSTATIC TEST\n" >> $(RAPORT)
	make clean
	make libstatic
	$(CC) -O0 -c main.c -o main.o
	$(CC) -O0 -static main.o -L. lib.a -o main
	./main test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean

	echo "\nSHARED TEST\n" >> $(RAPORT)
	make libshared

	$(CC) -O0 main.c -L. -Wl,-rpath=. -lmycode -o main
	./main test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean

	echo "\nDLL TEST\n" >> $(RAPORT)
	make libshared

	$(CC) -O0 main_dynamic.c -ldl -o main_dynamic
	./main_dynamic test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean
	make clean_tmp

	echo "\n OPTIMIZE -Os\n" >> $(RAPORT)
	echo "\nSTATIC TEST\n" >> $(RAPORT)
	make clean
	make libstatic
	$(CC) -Os -c main.c -o main.o
	$(CC) -Os -static main.o -L. lib.a -o main
	./main test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean

	echo "\nSHARED TEST\n" >> $(RAPORT)
	make libshared

	$(CC) -Os main.c -L. -Wl,-rpath=. -lmycode -o main
	./main test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean

	echo "\nDLL TEST\n" >> $(RAPORT)
	make libshared

	$(CC) -Os main_dynamic.c -ldl -o main_dynamic
	./main_dynamic test test1.txt test2.txt test3.txt >> $(RAPORT)
	make clean
	make clean_tmp








