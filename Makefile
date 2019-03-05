build/main: build/main.o build/transbank.o
	cc -o build/main build/main.o build/transbank.o -lserialport
build/main.o: src/main.c src/transbank.h
	cc -c -g src/main.c -o build/main.o
build/transbank.o: src/transbank.h src/transbank.c
	cc -c -g src/transbank.c -o buid/transbank.o

run: build/main.o
	./build/main
