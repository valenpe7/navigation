#include <stdio.h>
#include <stdlib.h>

#include "./inc/graph.h"

void swap(int *first, int *second) {
	int c;
	c = *first;
	*first = *second;
	*second = c;
return;
}

void InitializeGraph(Graph *graph) {
	if (!graph) {
		return;
	}
	graph->nodes = NULL;
	graph->n = 0;
	graph->capacity = 0;
	return;
}

void AddNode(Graph *graph, Node *node) {
	if(!graph || !node) {
		return;
	}
	graph->n++;
	if (graph->n >= graph->capacity) {
		graph->capacity += 5;
		graph->nodes = (Node*)realloc(graph->nodes, graph->capacity*sizeof(Node));
		if (!graph->nodes)
			return;
	}
	graph->nodes[graph->n - 1] = *node;
	return;
}

void AddEdge(Graph *graph, Edge *edge) {
	if (!graph || !edge) {
		return;
	}
	graph->nodes[edge->source].e++;
	if (graph->nodes[edge->source].e >= graph->nodes[edge->source].capacity) {
		graph->nodes[edge->source].capacity += 5;
		graph->nodes[edge->source].edges = (Edge*)realloc(graph->nodes[edge->source].edges, graph->nodes[edge->source].capacity*sizeof(Edge));
		if (!graph->nodes[edge->source].edges)
			return;
	}
	graph->nodes[edge->source].edges[graph->nodes[edge->source].e - 1] = *edge;
	if (edge->oneway == 0) {
		swap(&edge->source, &edge->target);
		edge->oneway = 1;
		AddEdge(graph, edge);
	}
	return;
}

int FindNode(Graph *graph, int id) {
	int i;
	if (!graph || !id)
		return -1;
	for (i = 0; i < graph->n; i++) {
		if (graph->nodes[i].id == id)
			return i;
	}
	return -1;
}

void FreeGraph(Graph *graph) {
	int i;
	if (!graph)
		return;
	for (i = 0; i < graph->n; i++) {
			free(graph->nodes[i].edges);
			free(graph->nodes[i].name);
	}
	free(graph->nodes);
	InitializeGraph(graph);
	return;
}



	

