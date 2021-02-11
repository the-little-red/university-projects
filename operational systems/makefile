CC=gcc
CFLAGS=-I ghi-g -Wall -Wextra -lm -lrt
OBJ= ppos_core.o ppos_disk.o queue.o hw_disk.o pingpong-disco.o

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS)

sotrabalho: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)
.PHONY: clean

clean:
		rm *.o

run:
	./teste
