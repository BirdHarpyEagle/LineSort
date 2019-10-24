all: linesort
linesort: main.o
	gcc main.o -o linesort
main.o:
	gcc -c main.c
clean:
	rm main.o
