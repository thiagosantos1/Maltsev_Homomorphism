CC     = gcc
HDR1   = ../include/graphs.h
OPT    = -g -Wall
CFLAGS = -c -I../include
OBJS   = readgraphs.o 

all: maltsev_run

maltsev_run: main.o readgraphs.o graphsconstruct.o maltsev.o
	$(CC) $(OPT) -o ../bin/$@ main.o readgraphs.o graphsconstruct.o maltsev.o

main.o: main.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) main.c
readgraphs.o: readgraphs.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) readgraphs.c
graphsconstruct.o: graphsconstruct.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) graphsconstruct.c

maltsev.o: maltsev.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) maltsev.c

clean:
	rm -f *.o ../bin/

run:
	../bin/./maltsev_run ../etc/graph_G.txt ../etc/graph_H.txt ../etc/list_file.txt 