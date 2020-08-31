CC = gcc

CFLAGS = -Wall -Werror -lm -g -std=c11

all: pagerank

pagerank: pagerank.o graph.o
	$(CC) pagerank.o graph.o -o pagerank

pagerank.o: pagerank.c pagerank.h graph.h
	$(CC) $(CFLAGS) pagerank.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) graph.c
clean:
	rm -rf *.o
