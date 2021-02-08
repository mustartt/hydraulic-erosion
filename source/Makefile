# Makefile for building the binary
# currently it builds to native machine executable
# later it will build to WASM and initiate streams with in the main page

CC=clang
CFLAGS=-lm

OBJDIR=obj

$(OBJDIR)/erosion.o: erosion.c erosion.h
	$(CC) -c erosion.c -o $(OBJDIR)/erosion.o

.PHONY: clean
clean:
	rm *.o 