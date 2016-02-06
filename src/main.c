#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./inc/graph.h"
#include "./inc/gml_parser.h"

const int capacity = 60;

typedef struct Pair {
	int src, dst;
	double length;
} Pair;

typedef struct ReturnStruct {
	double length;
	Pair *pairs;
	int capacity;
	int numPairs;
} ReturnStruct;

void save_node(Graph *graph, struct GML_pair *list) {
	struct GML_pair* tmp = list;
	Node p;
	int i;
	p.e = 0;
	p.capacity = 0;
	p.edges = NULL;
	while (tmp) {
		if (strcmp(tmp->key, "id") == 0)
			p.id = tmp->value.integer;
		if (strcmp(tmp->key, "label") == 0) {
			if (strlen(tmp->value.string) < capacity)
				p.name = strdup(tmp->value.string);
			else {
				p.name = (char*)malloc(capacity*sizeof(char));
				if (!p.name)
					return;
				for (i = 0; i < capacity; i++)
					p.name[i] = tmp->value.string[i];
			}
		}
		tmp = tmp->next;
	}
	AddNode(graph, &p);
	return;
}

void save_edge(Graph *graph, struct GML_pair *list) {
	struct GML_pair* tmp = list;
	Edge e;
	e.oneway = 0;
	while (tmp) {
		if (strcmp(tmp->key, "source") == 0)
			e.source = FindNode(graph, tmp->value.integer);
		if (strcmp(tmp->key, "target") == 0)
			e.target = FindNode(graph, tmp->value.integer);
		if (strcmp(tmp->key, "length") == 0) {
			switch (tmp->kind) {
				case GML_INT:
					e.length = (double)tmp->value.integer;
					break;
				case GML_DOUBLE:
					e.length = tmp->value.floating;
					break;
				default:
					break;
			}
		}
		if (strcmp(tmp->key, "type") == 0)
			e.type = tmp->value.integer;
		if (strcmp(tmp->key, "oneway") == 0)
			e.oneway = 1;
		tmp = tmp->next;
	}
	AddEdge(graph, &e);
	return;
}

void save_list(Graph *graph, struct GML_pair *list) {
	struct GML_pair* tmp = list;
	while (tmp) {
		if (strcmp(tmp->key, "graph") == 0) {
			InitializeGraph(graph);
			save_list(graph, tmp->value.list);
		}
		if (strcmp(tmp->key, "node") == 0)
			save_node(graph, tmp->value.list);
		if (strcmp(tmp->key, "edge") == 0)
			save_edge(graph, tmp->value.list);
		tmp = tmp->next;
	}
	return;
}

void Dijkstra (Graph *graph, char *source, char *target) {
	int i, j, k, s = -1, t = -1, u = -1, *b, *prev;
	double min, alt = 0, *dist, *length;
	Graph Q;
	ReturnStruct S;
	InitializeGraph(&Q);
	Q = *graph;
	S.pairs = (Pair*)malloc(sizeof(Pair));
	if (!S.pairs)
		return;
	S.length = 0;
	S.numPairs = 0;
	S.capacity = 0;
	dist = (double*)malloc(Q.n*sizeof(double));
	if (!dist) {
		free(S.pairs);
		return;
	}
	length = (double*)malloc(Q.n*sizeof(double));
	if (!length) {
		free(S.pairs);
		free(dist);
		return;
	}
	b = (int*)malloc(Q.n*sizeof(int));
	if (!b) {
		free(S.pairs);
		free(dist);
		free(length);
		return;
	}
	prev = (int*)malloc(Q.n*sizeof(int));
	if (!prev) {
		free(S.pairs);
		free(dist);
		free(length);
		free(b);
		return;
	}
	for (i = 0; i < Q.n; i++) {
		if (strcmp(Q.nodes[i].name, source) == 0)
			s = i;
		if (strcmp(Q.nodes[i].name, target) == 0)
			t = i;
		dist[i] = HUGE_VAL;
		length[i] = 0;
		b[i] = 0;
	}
	if (s == -1 || t == -1) {
		printf("Invalid starting or ending node");
		free(dist);
		free(b);
		free(prev);
		free(length);
		free(S.pairs);
		return;
		
	}
	dist[s] = 0;
	u = s;
	for (i = 0; i < Q.n; i++) {
		min = dist[u];
		while (b[u] != 0) {
			for (k = 0; k < Q.n; k++) {
				if (dist[k] < min && b[k] == 0) {
					u = k;
				}
			}
			min += 0.1;
		}
		if (dist[u] == HUGE_VAL || u == t)
			break;
		for (j = 0; j < Q.nodes[u].e; j++) {
			switch (Q.nodes[u].edges[j].type) {
			case 0:
				alt = dist[u] + Q.nodes[u].edges[j].length + 50;
				break;
			case 1:
				alt = dist[u] + Q.nodes[u].edges[j].length;
				break;
			case 2:
				alt = dist[u] + Q.nodes[u].edges[j].length - 50;
				break;
			default:
				break;
			}
			if (alt < 0)
				alt = 0;
			if (alt < dist[Q.nodes[u].edges[j].target] && b[Q.nodes[u].edges[j].target] == 0) {
				dist[Q.nodes[u].edges[j].target] = alt;
				prev[Q.nodes[u].edges[j].target] = u;
				length[Q.nodes[u].edges[j].target] = Q.nodes[u].edges[j].length;
			}
		}
		b[u] = 1;
	}
	while (u != s) {
		S.numPairs++;
		if (S.numPairs >= S.capacity) {
			S.capacity += 5;
			S.pairs = (Pair*)realloc(S.pairs, S.capacity*sizeof(Pair));
			if (!S.pairs) {
				free(S.pairs);
				break;
			}
		}
		S.pairs[S.numPairs - 1].src = prev[u];
		S.pairs[S.numPairs - 1].dst = u;
		S.pairs[S.numPairs - 1].length = length[u];
		S.length += length[u];
		u = prev[u];
	}
	while (S.numPairs > 0) {
		printf("road: %s -> %s (%.2f)\n", Q.nodes[S.pairs[S.numPairs - 1].src].name, Q.nodes[S.pairs[S.numPairs - 1].dst].name, S.pairs[S.numPairs - 1].length);
		S.numPairs--;
	}
	printf("total length: %.2f\n", S.length);
	free(dist);
	free(b);
	free(prev);
	free(length);
	free(S.pairs);
}

int main (int argc, char* argv[]) {
    struct GML_pair* list;
    struct GML_stat* stat;
	Graph roadmap;
	stat = (struct GML_stat*)malloc(sizeof(struct GML_stat));
	if (!stat)
		return -1;
    stat->key_list = NULL;
    if (argc != 4)
		printf("Usage: main.exe <gml_file> <\"source\"> <\"target\">\n");
    else {
		FILE* file = fopen (argv[1], "r");
		if (!file)
			printf ("\n No such file: %s", argv[1]);
		else {
			GML_init();
			list = GML_parser(file, stat, 0);
			if (stat->err.err_num != GML_OK) {
				printf ("An error occured while reading line %d column %d of %s:\n", stat->err.line, stat->err.column, argv[1]);
				switch (stat->err.err_num) {
					case GML_UNEXPECTED:
						printf ("UNEXPECTED CHARACTER");
						break;
					case GML_SYNTAX:
						printf ("SYNTAX ERROR"); 
						break;
					case GML_PREMATURE_EOF:
						printf ("PREMATURE EOF IN STRING");
						break;
					case GML_TOO_MANY_DIGITS:
						printf ("NUMBER WITH TOO MANY DIGITS");
						break;
					case GML_OPEN_BRACKET:
						printf ("OPEN BRACKETS LEFT AT EOF");
						break;
					case GML_TOO_MANY_BRACKETS:
						printf ("TOO MANY CLOSING BRACKETS");
						break;
					default:
						break;
				}
			}
			save_list(&roadmap, list);
			Dijkstra(&roadmap, argv[2], argv[3]);
			GML_free_list(list, stat->key_list);
			FreeGraph(&roadmap);
		}
		fclose(file);
    }
	free(stat);
	return 0;
}

