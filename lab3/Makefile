CC = gcc
CFLAG = -g -Wall
DATAFILE="data.dat"

all: hash

hash: hash.c
	$(CC) hash.c $(CFLAG) -o hash.out

draw: hash
	./hash.out $(DATAFILE)
	./plot.sh $(DATAFILE)

clean:
	rm -rf *.out *.dat *.png