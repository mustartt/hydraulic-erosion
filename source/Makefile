# Makefile for building the binary
# currently it builds to native machine executable
# later it will build to WASM and initiate streams with in the main page

CC=clang
CFLAGS=-lm -Wall

OBJDIR=build

$(OBJDIR)/erosion.o: erosion.c erosion.h
	$(CC) $(CFLAGS) -c erosion.c -o $(OBJDIR)/erosion.o

$(OBJDIR)/noise.o: noise.c noise.h
	$(CC) $(CFLAGS) -c noise.c -o $(OBJDIR)/noise.o

$(OBJDIR)/heightmap_gen.o: heightmap_gen.c heightmap_gen.h
	$(CC) $(CFLAGS) -c heightmap_gen.c -o $(OBJDIR)/heightmap_gen.o

$(OBJDIR)/export.o: export.c export.h
	$(CC) $(CFLAGS) -c export.c svpng.c -o $(OBJDIR)/export.o



.PHONY: clean
clean:
	rm *.o 