all: main.exe

main.exe: matriz.o main.o
	gcc -o $@ $^

Disco.o: matriz.c matriz.h
	gcc -c -o $@ $<

main.o: main.c matriz.h
	gcc -c -o $@ $<

clean:
	rm main.o
	rm main.exe
	rm matriz.o
