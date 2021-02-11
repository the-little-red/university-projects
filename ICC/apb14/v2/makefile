CC= gcc
CFLAGS =-std=c11 -g -Wall -Wno-unused-result -I/home/soft/likwid/include -llikwid -L/usr/lib -L/home/soft/likwid/lib -lm -std=c99 -Wall -DLIKWID_PERFMON -O3 -march=native -mavx -fstrict-aliasing
LFLAGS = -lm
OBJ= main.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDLIBS) $(LFLAGS)

cgSolver: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LDLIBS) $(LFLAGS)

.PHONY: clean

clean:
	rm *.o

doc: *.c t2.doxy *.h
	@doxygen t2.doxy

run:
	./cgSolver
