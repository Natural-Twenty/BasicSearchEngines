
#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct GraphRep {
    int nV;
    int nE;
    int **edges;  // matrix of weights (0 means no edge)
} GraphRep;
typedef struct GraphRep *Graph;

// Vertex is represented by an index.
typedef int Vertex;

typedef struct Edge {
	Vertex v;
	Vertex w;
} Edge;

// Check if Vertex is valid in a graph.
int validV (Graph g, Vertex v);

// Creates an edge between two vertices.
Edge createEdge (Graph g, Vertex v, Vertex w);

// Inserts an edge into a graph. Ignores outgoing connections to itself.
void insertEdge (Graph g, Vertex v, Vertex w, int weight);

// Checks whether there is a directed connection from v to w. Directed connection
// exists as long as it is not 0 since 0 is represented as no connection in this ADT.
bool isConnected (Graph g, Vertex v, Vertex w);

// Creates an empty graph using known number of vertices it will contain.
// It will allocate memory accordingly.
Graph createGraph (int nV);

// Free an array containing strings
void freeStrArray(char **str, int size);

// Free a graph
void freeGraph (Graph g);

// Generate a graph using urls from the collection.txt file
Graph generateGraph ();

// Finds out whether the url has an outgoing connection to a url in array.
// Returns position if the url file contains the name of other urls. Otherwise, 
// returns false.
int findOutLinkIndex(char *str, char **urlArray, int nURLs);

// Gets number of incoming links. Type is double because it will be used in a 
// calculation that uses double typing.
double getNumInLinks (Graph g, Vertex v);

// Gets number of outgoing links. Type is double because it will be used in a
// calculation that uses double typing.
double getNumOutLinks (Graph g, Vertex v);

// Finds the sum of incoming links to all nodes v references. Uses double typing 
// because it will be used in a calculation that uses double typing
double inLinkSum (Graph g, Vertex v);

// Finds the sum of all outgoing links to all nodes v references. Uses double typing 
// because it will be used in a calculation that uses double typing
double outLinkSum (Graph g, Vertex v);
#endif
