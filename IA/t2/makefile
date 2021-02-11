CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline

all: controlador

controlador: controlador.c

jogador: campo.h campo.c jogador.c

clean:
	rm *.o
