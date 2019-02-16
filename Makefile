CFLAGS = -Wall -g

all: mem

mem: mem.o
 gcc $(CFLAGS) mem.o -lm -o mem

mem.o:
 gcc $(CFLAGS) -c memory.c

clean:
 rm -rf *.o mem
