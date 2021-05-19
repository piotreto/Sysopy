build:
	gcc -g -pthread main.c -o main
clean:
	rm -f main
run:
	make build
	./main 10 numbers image1.pgm image2.pgm
	make clean

test:
	make build
	echo "\nTimes\n" > Times.txt
	./main 1 numbers image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 2 numbers image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 4 numbers image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 8 numbers image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 16 numbers image1.pgm image2.pgm >> Times.txt

	echo "\n\n\n" >> Times.txt
	
	./main 1 blocks image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 2 blocks image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 4 blocks image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 8 blocks image1.pgm image2.pgm >> Times.txt
	echo "\n" >> Times.txt
	./main 16 blocks image1.pgm image2.pgm >> Times.txt

	make clean


