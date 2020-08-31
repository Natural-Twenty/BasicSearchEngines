
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "graph.h"

#define LINE_MAX 1000
#define NO_OUTLINKS 0.5


/*  Check if Vertex whether vertex is valid
	Complexity: O(C)
	C is constant.
*/
int validV (Graph g, Vertex v) {
    return (g != NULL && v >= 0 && v < g->nV);
}

/* 	Create an edge
	Complexity: O(C)
	C is constant.
*/
Edge createEdge (Graph g, Vertex v, Vertex w) {
    Edge new = {v,w};  // struct assignment
    return new;
}


/*  Inserts an edge. Ignores outgoing connections to itself.
	Complexity: O(C)
	C is constant.
*/
void insertEdge (Graph g, Vertex v, Vertex w, int weight) {
    if (v == w) {return;}
    if (g->edges[v][w] == 0) {
        g->edges[v][w] = weight;
        g->nE++;
    }
}

/*  Checks whether there is a directed connection from v to w. Directed
	connection exists as long as it is not 0 since 0 is represented as no
	connection in this ADT.
	Complexity: O(C)
	C is constant.
*/
bool isConnected (Graph g, Vertex v, Vertex w) {
    return (g->edges[v][w] != 0);
}

/*  Creates an empty graph using known number of vertices it will contain.
	It will allocate memory accordingly.
	Complexity: O(n^2)
	n = number of vertices
*/
Graph createGraph (int nV) {
    Graph new = malloc(sizeof(*new));
    new->nV = nV;
    new->nE = 0;
    new->edges = malloc(nV*sizeof(int *));
    int v, w;
    for (v = 0; v < nV; v++) {
        new->edges[v] = malloc(nV*sizeof(int));
        for (w = 0; w < nV; w++) {
            new->edges[v][w] = 0;
        }
    }
    return new;
}

/*  Free an array containing strings
	Complexity: O(n)
	n = number of vertices
*/
void freeStrArray(char **str, int size) {
    for (int i = 0; i < size; i++) {
        free(str[i]);
    }
    free(str);
}

/*  Free a graph
	Complexity: O(n)
	n = number of vertices
*/
void freeGraph (Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

/*  Generate a graph using urls from the collection.txt file and will record
	all edges by reading outgoing links from url files.
	Complexity: O(m * n^2)
	n = number of vertices
	m = number of words in url file
*/
Graph generateGraph () {
    FILE *fp;
    fp = fopen("collection.txt", "r");
    char buffer[LINE_MAX];
    char **urlArray = NULL;
    int nURLs = 0;
    
    while (fscanf(fp, "%s", buffer) != EOF) {
        urlArray = realloc(urlArray, (nURLs+1)*sizeof(char *));
        urlArray[nURLs] = malloc(strlen(buffer)+1);
        strcpy(urlArray[nURLs], buffer);
        nURLs++;
    }
    
    fclose(fp);
    
    Graph g = createGraph(nURLs);
    
    // Concatenate .txt to each url
    char tempBuf[LINE_MAX];
    int v;
    for (v = 0; v < nURLs; v++) {
        char url[LINE_MAX];
        char concatStr[LINE_MAX];
        strcpy(url, urlArray[v]);
        strcpy(concatStr, ".txt");
        char *fileName = strcat(url, concatStr);
        
        fp = fopen(fileName, "r");
        
        while (fscanf(fp, "%s", tempBuf) != EOF) {
        	int outLinkIndex = findOutLinkIndex(tempBuf, urlArray, nURLs);
            if (outLinkIndex >= 0) {
                
                insertEdge(g, v, outLinkIndex, 1);
            }
        }
    	fclose(fp);    
    }
    
    freeStrArray(urlArray, nURLs);
    return g;
}
        
    
/*  Finds out whether the url has an outgoing connection to a url in array.
    Returns position if the url file contains the name of other urls. Otherwise, 
    returns false.
	Complexity: O(n)
	n = number of vertices, which is also the number of urls.
*/
int findOutLinkIndex(char *str, char **urlArray, int nURLs) {
	int i;
    for (i = 0; i < nURLs; i++) {
        if (strcmp(str, urlArray[i]) == 0) {
            return i;
        }
    }
    return -1;
}
      
/*  Gets number of incoming links. Type is double because it will be used in a 
    calculation that uses double typing.
    Complexity: O(n)
    n = number of vertices
*/
double getNumInLinks (Graph g, Vertex v) {
    double inLinks = 0;
    int i;
    for (i = 0; i < g->nV; i++) {
        // Use 'not zero' logic because 0 represets no link in this ADT
        if (g->edges[i][v] != 0) {
            inLinks++;
        }
    }
    return inLinks;
}

/*  Gets number of outgoing links. Type is double because it will be used in a
    calculation that uses double typing.
    Complexity: O(n)
    n = number of vertices
*/
double getNumOutLinks (Graph g, Vertex v) {
    double outLinks = 0;
    int i;
    for (i = 0; i < g->nV; i++) {
        // Use 'not zero' logic because 0 represents no link in this ADT
        if (g->edges[v][i] != 0) {
            outLinks++;
        }
    }
    // no outlinks is considered to be 0.5 to avoid division by 0.
    if (outLinks == 0) {
    	return NO_OUTLINKS;
    } else {
    	return outLinks;
    }
}
/*  Finds the sum of incoming links to all nodes v references. Uses double typing 
    because it will be used in a calculation that uses double typing.
    Complexity: O(n)
    n = number vertices
*/
double inLinkSum (Graph g, Vertex v) {
    double sum = 0;
    for (int i = 0; i < g->nV; i++) {
        if (g->edges[v][i] != 0) {
            sum = sum + getNumInLinks(g, i);
        }
    }
    return sum;
}
    
/*  Finds the sum of all outgoing links to all nodes v references. Uses double typing 
    because it will be used in a calculation that uses double typing.
    Complexity: O(n)
    n = number of vertices
*/
double outLinkSum (Graph g, Vertex v) {
    double sum = 0;
    for (int i = 0; i < g->nV; i++) {
        if (g->edges[v][i] != 0) {
            sum = sum + getNumOutLinks(g, i);
        }
    }
    return sum;
}
    
    
