
#ifndef PAGERANK_H
#define PAGERANK_H

#include <stdio.h>

// Calculates page rank and returns the array of page ranks.
double *calculatePageRank (Graph g, double d, double diffPR, int maxIt);

// Bubble sort pageRank array in descending order.
// Based of internet search and videos
void bubbleSort (double *PR, char **urlArray, int *nOutLinks, int nV);

// Opens an output file and writes the url, number of outgoing links and page rank.
void writeToOutputFile (double *PR, char **urlArray, int *nOutLinks, int nV);

#endif
