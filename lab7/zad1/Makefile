CC =  gcc 


build:
	touch bake
	touch table
	touch pizzeria
	$(CC) -o chief chief.c sem.c
	$(CC) -o supplier supplier.c sem.c
	$(CC) -o main main.c sem.c


run:
	make build
	./main 6 5
	make clean

clean:
	rm -f main chief supplier
	rm -f bake table pizzeria