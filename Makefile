CFLAGS = -ansi -pedantic -Wall -pthread 

all:assign2

assign2: semaphors.c 
	gcc $(CFLAGS) semaphors.c;
	./a.out;
clean:
	rm -rf a.out;
