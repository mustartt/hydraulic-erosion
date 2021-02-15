# Main Make file
# use make web for emcc to WASM
# use make local for clang to local executable

wasm:
	make -f makefile-emcc.mk

local:
	make -f makefile-clang.mk

clean:
	-make -f makefile-clang.mk clean
	-make -f makefile-emcc.mk clean

clean-win:
	-make -f makefile-clang.mk clean-win
	-make -f makefile-emcc.mk clean-win