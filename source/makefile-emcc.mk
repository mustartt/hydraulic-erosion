# Makefile for compiling the wasm files

CC=emcc
CFLAGS=-Wall -O3

output.js: api.o erosion.o noise.o heightmap_gen.o utils.o
	$(CC) $(CFLAGS) api.o erosion.o noise.o heightmap_gen.o utils.o -o output.js \
		-s EXPORTED_FUNCTIONS='["_calloc", "_malloc", "_free"]' \
		-s WASM=1 \
		-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
		-s 'EXTRA_EXPORTED_RUNTIME_METHODS=["FS"]' \
		-s NO_EXIT_RUNTIME=1 \
		-s ENVIRONMENT='web' 

# build object files
api.o: api.c api.h
	$(CC) $(CFLAGS) -c api.c -o api.o -D _WASM

erosion.o: erosion.c erosion.h
	$(CC) $(CFLAGS) -c erosion.c -o erosion.o

noise.o: noise.c noise.h
	$(CC) $(CFLAGS) -c noise.c -o noise.o

heightmap_gen.o: heightmap_gen.c heightmap_gen.h
	$(CC) $(CFLAGS) -c heightmap_gen.c -o heightmap_gen.o

utils.o: export.c export.h
	$(CC) $(CFLAGS) -c export.c -o utils.o


.PHONY: clean
clean:
	-rm *.o
	-rm *.wasm
	-rm *.js
	-rm output
	-rm *.png
	-rm *.obj