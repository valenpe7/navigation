#ifndef _GRAPH_
#define _GRAPH_

typedef struct {
	int source;
	int target;
	double length;
	int type;
	int oneway;
} Edge;

typedef struct {
	int capacity;
	int e;
	int id;
	char *name;
	Edge *edges;
} Node;

typedef struct {
	int capacity;
	int n;
	Node *nodes;
} Graph;

void swap(int *first, int *second);
void InitializeGraph(Graph *graph);
void AddNode(Graph *graph, Node *node);
void AddEdge(Graph *graph, Edge *edge);
int FindNode(Graph *graph, int id);
void FreeGraph(Graph *graph);

#endif