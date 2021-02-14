# Makefile for building local executable binary
# currently it builds to native machine executable
# For building to WASM use make web

CC=clang
CFLAGS=-Wall -O3
CLIB=-lpng

OBJDIR=build

output: test.o erosion.o noise.o heightmap_gen.o utils.o import.o
	$(CC) $(CFLAGS) $(CLIB) test.o erosion.o noise.o heightmap_gen.o utils.o import.o -o output

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

# requires libpng to be installed 
#    linkes with libpng library
import.o: import.c import.h
	$(CC) $(CFLAGS) -c import.c -o import.o

.PHONY: clean
clean:
	rm *.o
	rm output
	rm *.png
	rm *.obj
