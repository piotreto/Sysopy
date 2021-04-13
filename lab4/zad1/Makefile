CC =  gcc 

clean:
	rm -f main1a
	rm -f main1b
	rm -f exec
zad1a:
	$(CC) -o main1a main1a.c
test1a_handler:
	make clean
	make zad1a
	./main1a handler
	make clean
test1a_mask:
	make clean
	make zad1a
	./main1a mask
	make clean
test1a_ignore:
	make clean
	make zad1a
	./main1a ignore
	make clean
test1a_pending:
	make clean
	make zad1a
	./main1a pending
	make clean


zad1b:
	$(CC) -o exec exec.c
	$(CC) -o main1b main1b.c

test1b_handler:
	make clean
	make zad1b
	./main1b handler
	make clean
test1b_mask:
	make clean
	make zad1b
	./main1b mask
	make clean
test1b_ignore:
	make clean
	make zad1b
	./main1b ignore
	make clean
test1b_pending:
	make clean
	make zad1b
	./main1b pending
	make clean