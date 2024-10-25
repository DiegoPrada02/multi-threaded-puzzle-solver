CC=gcc
CFLAGS=-I. -g -w -pthread -lm
DEPS= solve.h
OBJ = solver.o hashset.o btree.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
solve: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)