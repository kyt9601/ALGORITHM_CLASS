#include <stdio.h>
#include <stdlib.h>

struct vertex;
struct edge;
struct ioedge;
struct graph;

typedef enum { FRESH, VISITED } status;

typedef struct vertex
{
	int name;
	struct ioedge *inedges;
	struct ioedge *outedges;

	int TPOrder;
	status st;
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

void TPSortDFS(graph *pg, int n);
int rTPSortDFS(graph *pg, vertex *pv, int N);
vertex *find(graph *pg, int n, int index);

int main()
{
	graph g;
	int n, m;

	scanf("%d %d", &n, &m);
	makeGraph(&g, n, m);

	TPSortDFS(&g, n);

	for (int i=1; i<=n; i++)
		printf("%d\n",find(&g, n, i)->name);

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

void TPSortDFS(graph *pg, int n)
{
	int N;

	for (int i=0; i<n; i++)
		pg->verticies[i].st = FRESH;

	N = n;
	for (int i=0; i<n; i++)
		if (pg->verticies[i].st == FRESH)
			N = rTPSortDFS(pg, pg->verticies + i, N);
}

int rTPSortDFS(graph *pg, vertex *pv, int N)
{
	pv->st = VISITED;

	for (ioedge *p=pv->outedges->next; p!=NULL; p=p->next)
		if (p->pe->dest->st == FRESH)
			N = rTPSortDFS(pg, p->pe->dest, N);

	pv->TPOrder = N--;

	return N;
}

vertex *find(graph *pg, int n, int index)
{
	for (int i=0; i<n; i++)
		if (pg->verticies[i].TPOrder == index)
			return pg->verticies + i;
}