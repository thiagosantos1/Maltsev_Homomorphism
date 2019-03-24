CC     = gcc #-lm
HDR1   = ../include/graphs.h
OPT    = -g -Wall
CFLAGS = -c -I../include
OBJS   = readgraphs.o 

all: maltsev_run construct_G_H

maltsev_run: main.o readgraphs.o graphsconstruct.o pre_processing.o maltsev.o
	$(CC) $(OPT) -o ../bin/$@ main.o readgraphs.o graphsconstruct.o pre_processing.o  maltsev.o

construct_G_H: construct_randG_H.o
	$(CC) $(OPT) -o ../bin/$@  construct_randG_H.o 


main.o: main.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) main.c
readgraphs.o: readgraphs.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) readgraphs.c
graphsconstruct.o: graphsconstruct.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) graphsconstruct.c 
pre_processing.o: pre_processing.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) pre_processing.c
maltsev.o: maltsev.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) maltsev.c
construct_randG_H.o: construct_randG_H.c $(HDR1)
	$(CC) $(CFLAGS) $(OPT) construct_randG_H.c 

clean:
	rm -f *.o ../bin/*

G_H:
	@#@echo $$numV
	@#@echo $$prob
	@#make run numV=10
	@../bin/./construct_G_H $$numVG $$probG $$probH

run:
	../bin/./maltsev_run ../etc/graph_G.txt ../etc/graph_H.txt ../etc/list_file.txt 



