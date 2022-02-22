CC = gcc

all: master slave
%.o: %.c
	$(CC) -c -std=gnu99 $<
master: master.o
	$(CC) -o master master.o
slave: slave.o
	$(CC) -o slave slave.o
clean:
	rm master slave *.o