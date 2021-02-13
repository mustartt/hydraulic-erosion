# Makefile for building the binary
# currently it builds to native machine executable
# later it will build to WASM and initiate streams with in the main page

CC=clang
CFLAGS=-lm -Wall

OBJDIR=build

output: test.o erosion.o noise.o heightmap_gen.o utils.o
	$(CC) $(CFLAGS) test.o erosion.o noise.o heightmap_gen.o utils.o -o output

erosion.o: erosion.c erosion.h
	$(CC) $(CFLAGS) -c erosion.c -o erosion.o

noise.o: noise.c noise.h
	$(CC) $(CFLAGS) -c noise.c -o noise.o

heightmap_gen.o: heightmap_gen.c heightmap_gen.h
	$(CC) $(CFLAGS) -c heightmap_gen.c -o heightmap_gen.o

utils.o: export.c export.h
	$(CC) $(CFLAGS) -c export.c -o utils.o

test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

.PHONY: clean
clean:
	rm erosion.o
	rm noise.o
	rm heightmap_gen.o
	rm utils.o
	rm test.o
	rm output
