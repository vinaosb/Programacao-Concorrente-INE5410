CC=gcc

FLAGS=-O3 -Wno-unused-result
LDFLAGS=-lpthread -lm
#DEBUG=-DDEBUG
#RESULT=-DRESULT

all: gol golOriginal

gol: gol.c
	$(CC) $(DEBUG) $(RESULT) $(FLAGS)  gol.c -o gol $(LDFLAGS)
golOriginal: golOriginal.c
	$(CC) $(DEBUG) $(RESULT) $(FLAGS)  golOriginal.c -o golOriginal $(LDFLAGS)

clean:
	rm -rf gol golOriginal
