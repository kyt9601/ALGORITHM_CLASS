#include <stdio.h>
#include <stdlib.h>

struct vertex;
struct edge;
struct ioedge;
struct graph;

typedef struct vertex
{
	int name;
	struct ioedge *inedges;
	struct ioedge *outedges;
} vertex;

typedef struct edge
{
	vertex *origin;
	vertex *dest;
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

int main()
{
	graph g;
	int n, m;

	scanf("%d %d", &n, &m);
	makeGraph(&g, n, m);

	for (int i=0; i<n; i++)
	{
		printf("%d", i+1);

		for (ioedge *p=g.verticies[i].outedges->next; p!=NULL; p=p->next)
			printf(" %d", p->pe->dest->name);
		puts("");
	}
	
	cleanUp(&g, n);

	return 0;
}

void makeGraph(graph *pg, int n, int m)
{
	int origin, dest;

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
		scanf("%d %d", &origin, &dest);

		pg->edges[i].origin = pg->verticies + origin - 1;
		pg->edges[i].dest = pg->verticies + dest - 1;
		
		addInEdge(pg->verticies + dest - 1, pg->edges + i);
		addOutEdge(pg->verticies + origin - 1, pg->edges + i);
	}
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