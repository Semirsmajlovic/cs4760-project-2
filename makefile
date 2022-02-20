all: master slave

%.o: %.c 
	$(CC) -c -std=gnu99 $<

master: master.o
	gcc -o master master.o
	
slave: slave.o
	gcc -o slave slave.o

clean:
	rm master slave *.o