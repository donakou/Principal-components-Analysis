CC=gcc
all: executable
executable: main.o matrice.o
	$(CC) -o  executable main.o matrice.o -lm
matrice.o: matrice.c matrice.h
	$(CC) -c  matrice.c
main.o: main.c
	$(CC) -c  main.c

clean:
	rm main.o matrice.o executable 
