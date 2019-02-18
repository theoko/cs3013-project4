CFLAGS = -Wall -g

all: mem

mem: mem.o
	gcc $(CFLAGS) memory.o -lm -o mem

mem.o:
	gcc $(CFLAGS) -c memory.c

clean:
	rm -rf *.o mem
