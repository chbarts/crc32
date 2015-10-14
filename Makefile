CC=gcc
CFLAGS=-O3 -march=native -flto
DEPS=handle_ferr.h crc32.h
OBJ=main.o crc32.o handle_ferr.o

all: crc32

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

crc32: main.o crc32.o handle_ferr.o
	$(CC) $(OBJ) -o crc32 -flto

clean:
	rm crc32 *.o
