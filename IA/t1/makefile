CC=gcc
CFLAGS= -g -Wall -Wno-unused-result -O3
OBJ= main.o

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS)

floodit: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
			rm *.o

run:
		./teste
