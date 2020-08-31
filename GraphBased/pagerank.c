
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graph.h"
#include "pagerank.h"

#define LINE_MAX 1000

/*  Program to calculate a simplified version of page rank for each url and
 	write it to an output file. 
 	Also includes number of outgoing links for each url.
	Complexity: O(m * n^2)
	m = number of words (content) in a url file.
	n = number of vertices, which is also the number of urls
	Taken from highest complexity function.
*/
int main (int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s dampingFactor diffPRsum, maxIterations\n", argv[0]);
        exit(1);
    }
    // Convert command line arguments into the appropriate type
    double dampingFactor = atof(argv[1]);
    double diffPRsum = atof(argv[2]);
    int maxIt = atoi(argv[3]);
   
    // Open file and read urls
    FILE *fp;
    fp = fopen("collection.txt", "r");
    char buffer[LINE_MAX];
    char **urlArray = NULL;
    int nURLs = 0;
    // Take URL names and copy to url array
    while (fscanf(fp, "%s", buffer) != EOF) {
        urlArray = realloc(urlArray, (nURLs+1)*sizeof(char *)); 
        urlArray[nURLs] = malloc(strlen(buffer)+1); 
        strcpy(urlArray[nURLs], buffer);
        nURLs++;
    }
    // Close file.
    fclose(fp);
    
    Graph g = generateGraph();
    
    double *pageRankArray = malloc((g->nV)*sizeof(double));
    pageRankArray = calculatePageRank(g, dampingFactor, diffPRsum, maxIt);
    
    int nOutLinks[nURLs];
    int i;
    for (i = 0; i < nURLs; i++) {
        nOutLinks[i] = getNumOutLinks(g, i);
    }
    
    bubbleSort(pageRankArray, urlArray, nOutLinks, nURLs);
    
    writeToOutputFile(pageRankArray, urlArray, nOutLinks, nURLs);
    
    freeStrArray(urlArray, nURLs);
    freeGraph(g);
    free(pageRankArray);
    
    return 0;
}    
    
/*  Calculates page rank and returns the array of page ranks.
	Complexity: O(n^2)
	n = number of vertices.
*/
double *calculatePageRank (Graph g, double d, double diffPR, int maxIt) {
	// Initialise variables to set up initial pagerank array
    int nV = g->nV;
    double nVdouble = (double) nV;
    double *pageRank = malloc(nV*sizeof(double));
    double prevPR[nV];
    int i, j;
    for (i = 0; i < nV; i++) {
        pageRank[i] = (double) (1/nVdouble);
    }
    
    // Initialise variables to calculate pageRank.
    int iterations = 0;
    double diffSum = 0;
    double diff = diffPR;
    double sigmaPR;
    double wIn;
    double wOut;
    // Stop when max iterations or when difference is insignificant enough.
    while (iterations < maxIt && diff >= diffPR) {
        // reset diffSum to 0 for each iteration
        diffSum = 0;
        // Store previous iteration's PR in prevPR array.
        for (i = 0; i < nV; i++) {
            prevPR[i] = pageRank[i];
        }
        for (i = 0; i < nV; i++) {
            //reset sigma expression for each loop.
            sigmaPR = 0;
            for (j = 0; j < nV; j++) {
                if (isConnected(g, j, i)) {
                    wIn = ( getNumInLinks(g, i)/inLinkSum(g,j) );
                    wOut = ( getNumOutLinks(g, i)/outLinkSum(g, j) );
                    sigmaPR += (prevPR[j]*wIn*wOut);
                }
            }
            // Update pageRank array for the vertice
            pageRank[i] = (double)((1-d)/nVdouble)+(d*sigmaPR);
        }
        // Update diffSum
        for (i = 0; i < nV; i++) {
            diffSum += fabs(pageRank[i] - prevPR[i]);
        }
        
        diff = diffSum;
        iterations++;
    }
    return pageRank;
}

/*  Bubble sort pageRank array in descending order of page rank.
    Based on internet search and videos
    Complexity: O(n^2)
    n = number of vertices
*/    
void bubbleSort (double *PR, char **urlArray, int *nOutLinks, int nV) {
	// Initialise variables
    int i, j;
    double tempPR;
    char tempStr[LINE_MAX];
    char tempIndex;
    for (i = 0; i < nV; i++) {
        for (j = i+1; j < nV; j++) {
            if (PR[i] < PR[j]) {
                // Swap all the data fields.
                tempPR = PR[i];
                PR[i] = PR[j];
                PR[j] = tempPR;
                
                strcpy(tempStr, urlArray[i]);
                strcpy(urlArray[i], urlArray[j]);
                strcpy(urlArray[j], tempStr);
                
                tempIndex = nOutLinks[i];
                nOutLinks[i] = nOutLinks[j];
                nOutLinks[j] = tempIndex;
            }
        }
    }
}

/*  Opens an output file and writes the url, number of outgoing links
    and page rank.
    Complexity: O(n)
    n = number of vertices.
*/
void writeToOutputFile (double *PR, char **urlArray, int *nOutLinks, int nV) {
    FILE *fp;
    fp = fopen("pagerankList.txt", "w");
    int i;
    for (i = 0; i < nV; i++) {
        fprintf(fp, "%s, %d, %.7f\n", urlArray[i], nOutLinks[i], PR[i]);
    }
    fclose(fp);
}
    

