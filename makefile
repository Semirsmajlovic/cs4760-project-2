CC = gcc

all: master slave

master: master.c
	$(CC) -o $@ $< -g -I -std=gnu99

slave: slave.c
	$(CC) -o $@ $< -g -I -std=gnu99

clean:
	rm cstest logfile.* master slave