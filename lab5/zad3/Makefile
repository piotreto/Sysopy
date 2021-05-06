CC =  gcc -Wall -std=c11 -g

clean:
	rm -f main
build:
	$(CC) -o consumer consumer.c
	$(CC) -o producer producer.c
test:
	make build
	./producer fifo 2 a.txt 5
	./producer fifo 1 b.txt 5
	./producer fifo 3 c.txt 5
	./consumer fifo result.txt 5
