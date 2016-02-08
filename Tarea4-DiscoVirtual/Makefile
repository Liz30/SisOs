all: main.exe

main.exe: Disco.o main.o
	gcc -o $@ $^

Disco.o: Disco.c Disco.h
	gcc -c -o $@ $<

main.o: main.c Disco.h
	gcc -c -o $@ $<

clean:
	rm main.o
	rm main.exe
	rm Disco.o
	rm disc1
