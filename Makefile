build/main: build/main.o build/transbank.o
	cc -o build/main build/main.o build/transbank.o -lserialport
build/main.o: src/main.c src/transbank.h
	cc -c -g src/main.c -o build/main.o
build/transbank.o: src/transbank.h src/transbank.c
	cc -c -g src/transbank.c -o build/transbank.o
run: src/main.c src/transbank.h src/transbank.c
	./build/main
debug: src/main.c src/transbank.h src/transbank.c
	export LIBSERIALPORT_DEBUG=1 && ./build/main && unset LIBSERIALPORT_DEBUG

wraper:
	swig -csharp -o build/transbank_wrap.c -namespace Transbank.pos src/transbank.i
	cd build && cc -fpic -c ../src/transbank.c transbank_wrap.c -I../src
	cc -dynamiclib build/transbank.o build/transbank_wrap.o -o build/transbank.dylib -lserialport
	sudo cp build/transbank.dylib /usr/local/lib
clean:
	rm -rf build/*
