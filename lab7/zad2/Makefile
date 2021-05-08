CC=gcc -Wall

build:
	$(CC) -o chef chef.c sem.c -lrt -lpthread
	$(CC) -o supplier supplier.c sem.c -lrt -lpthread
	$(CC) -o main main.c sem.c -lrt -lpthread


clean:
	rm -f main chef supplier
	rm -f bake table pizzeria

run:
	make build
	./main 5 5
	make clean