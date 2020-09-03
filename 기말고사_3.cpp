#include <stdio.h>
#include <stdlib.h>

#define INFINITE 0x6fffffff

struct vertex;
struct edge;
struct ioedge;
struct graph;

typedef struct vertex
{
	int name;
	struct ioedge *inedges;
	struct ioedge *outedges;

	int distance;
	int TPOrder;
} vertex;

typedef struct edge
{
	vertex *origin;
	vertex *dest;
	int weight;
} edge;

typedef struct ioedge
{
	edge *pe;
	struct ioedge *next;
} ioedge;

typedef struct graph
{
	vertex *verticies;
	edge *edges;
} graph;

void makeGraph(graph *pg, int n, int m);
ioedge *getIOEdge();
void addInEdge(vertex *pv, edge *pe);
void addOutEdge(vertex *pv, edge *pe);
void cleanUp(graph *pg, int n);

void DAGShortestPaths(graph *pg, int n, int s);
vertex *find(graph *pg, int n, int index);

int main()
{
	graph g;
	int n, m, s;

	scanf("%d %d %d", &n, &m, &s);
	makeGraph(&g, n, m);

	DAGShortestPaths(&g, n, s);

	for (int i=0; i<n; i++)
		if (i+1 != s && g.verticies[i].distance < INFINITE)
			printf("%d %d\n", i+1, g.verticies[i].distance);

	cleanUp(&g, n);

	return 0;
}

void makeGraph(graph *pg, int n, int m)
{
	int origin, dest, weight;

	pg->verticies = (vertex*)malloc(sizeof(vertex) * n);
	pg->edges = (edge*)malloc(sizeof(edge) * m);

	for (int i=0; i<n; i++)
	{
		pg->verticies[i].name = i + 1;
		pg->verticies[i].inedges = getIOEdge();
		pg->verticies[i].inedges->next = NULL;
		pg->verticies[i].outedges = getIOEdge();
		pg->verticies[i].outedges->next = NULL;
	}

	for (int i=0; i<m; i++)
	{
		scanf("%d %d %d", &origin, &dest, &weight);

		pg->edges[i].origin = pg->verticies + origin - 1;
		pg->edges[i].dest = pg->verticies + dest - 1;
		pg->edges[i].weight = weight;
		
		addInEdge(pg->verticies + dest - 1, pg->edges + i);
		addOutEdge(pg->verticies + origin - 1, pg->edges + i);
	}

	for (int i=0; i<n; i++)
		scanf("%d", &(pg->verticies[i].TPOrder));
}

ioedge *getIOEdge()
{
	return (ioedge*)malloc(sizeof(ioedge));
}

void addInEdge(vertex *pv, edge *pe)
{
	ioedge *p, *q;
	ioedge *nw = getIOEdge();
	nw->pe = pe;

	q = pv->inedges;
	for (p=q->next; p!=NULL && p->pe->origin->name < pe->origin->name; p=q->next)
		q = q->next;

	nw->next = q->next;
	q->next = nw;
}

void addOutEdge(vertex *pv, edge *pe)
{
	ioedge *p, *q;
	ioedge *nw = getIOEdge();
	nw->pe = pe;

	q = pv->outedges;
	for (p=q->next; p!=NULL && p->pe->dest->name < pe->dest->name; p=q->next)
		q = q->next;

	nw->next = q->next;
	q->next = nw;
}

void cleanUp(graph *pg, int n)
{
	ioedge *p, *q;

	for (int i=0; i<n; i++)
	{
		for (p=pg->verticies[i].inedges; p!=NULL; p=q)
		{
			q = p->next;
			free(p);
		}

		for (p=pg->verticies[i].outedges; p!=NULL; p=q)
		{
			q = p->next;
			free(p);
		}
	}

	free(pg->verticies);
	free(pg->edges);
}

void DAGShortestPaths(graph *pg, int n, int s)
{
	for (int i=0; i<n; i++)
		pg->verticies[i].distance = INFINITE;
	pg->verticies[s-1].distance = 0;

	for (int i=1; i<=n; i++)
	{
		vertex *pv = find(pg, n, i);

		for (ioedge *p=pv->outedges->next; p!=NULL && pv->distance!=INFINITE; p=p->next)
			if (pv->distance + p->pe->weight < p->pe->dest->distance)
				p->pe->dest->distance = pv->distance + p->pe->weight;
	}
}

vertex *find(graph *pg, int n, int index)
{
	for (int i=0; i<n; i++)
		if (pg->verticies[i].TPOrder == index)
			return pg->verticies + i;
}