CC=gcc
CFLAGS=-I ghi
OBJ= main2.o queue.o

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS)

token: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
		rm *.o
run:
	./teste
