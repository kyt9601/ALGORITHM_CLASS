#include <stdio.h>
#include <stdlib.h>

typedef struct edge
{
	int v1;
	int v2;
	int weight;
	struct edge *prev;
	struct edge *next;
} edge;

typedef struct incident
{
	edge *pe;
	struct incident *prev;
	struct incident *next;
} incident;

typedef struct
{
	int name;
	incident *incidHead;
} vertex;

typedef struct
{
	vertex vertices[6];
	edge *edgesHead;
} graph;

incident *getIncident();
edge *getEdge();
int opposite(vertex *pv, edge *pe);
void add_incident(vertex *pv, edge *pe);
void add_edge(graph *pg, int v1, int v2, int weight);
void delete_incident(vertex *pv, edge *pe);
void delete_edge(graph *pg, int v1, int v2);
edge *find_edge(edge *head, int v1, int v2);
void init(graph *pg);
void printAdjacent(vertex *pv);
void modifyEdge(graph *pg, int v1, int v2, int weight);

void cleanUp(graph *pg);

int main()
{
	char op;
	graph myGraph;
	
	init(&myGraph);
	
	while ( (op=getchar()) != 'q' )
	{
		int v1, v2;
		int vName;
		int weight;
		
		switch(op)
		{
		case 'a' :
			scanf("%d",&vName);
			if (vName<1 || 6<vName) { puts("-1"); }
			else { printAdjacent(&(myGraph.vertices[vName-1])); }
		break;
		case 'm' :
			scanf("%d %d %d",&v1,&v2,&weight);
			if ((v1<1 || 6<v1) || (v2<1 || 6<v2)) { puts("-1"); }
			else { modifyEdge(&myGraph,v1,v2,weight); }
		break;
		}
		
		getchar();
	}

	cleanUp(&myGraph);
	
	return 0;
}

incident *getIncident()
{
	return (incident*)malloc(sizeof(incident));
}

edge *getEdge()
{
	return (edge*)malloc(sizeof(edge));
}

int opposite(vertex *pv, edge *pe)
{
	if (pv->name == pe->v1)
		return pe->v2;
	else
		return pe->v1;
}

void add_incident(vertex *pv, edge *pe)
{
	incident *p;
	incident *nw;
	
	for (p=pv->incidHead->next; p->next!=NULL && opposite(pv,p->pe)<opposite(pv,pe); p=p->next);
	
	nw = getIncident();
	nw->pe = pe;

	nw->next = p->prev->next;
	nw->prev = p->prev;
	p->prev->next = nw;
	p->prev = nw;
}

void add_edge(graph *pg, int v1, int v2, int weight)
{
	edge *nw = getEdge();
	
	nw->v1 = v1;
	nw->v2 = v2;
	nw->weight = weight;

	nw->next = pg->edgesHead->next;
	nw->prev = pg->edgesHead->next->prev;
	pg->edgesHead->next->prev = nw;
	pg->edgesHead->next = nw;
	
	if (v1 != v2)
		add_incident(&(pg->vertices[v1-1]),nw);
	add_incident(&(pg->vertices[v2-1]),nw);
}

void delete_incident(vertex *pv, edge *pe)
{
	incident *p;

	for (p=pv->incidHead->next; p->pe!=pe; p=p->next);
	
	p->prev->next = p->next;
	p->next->prev = p->prev;

	free(p);
}

void delete_edge(graph *pg, int v1, int v2)
{
	edge *p = find_edge(pg->edgesHead,v1,v2);
	
	p->prev->next = p->next;
	p->next->prev = p->prev;

	if (v1 != v2)
		delete_incident(&(pg->vertices[v1-1]),p);
	delete_incident(&(pg->vertices[v2-1]),p);

	free(p);
}

edge *find_edge(edge *head, int v1, int v2)
{
	edge *pe = head->next;

	while (1)
	{
		if ( pe->next==NULL || (pe->v1==v1 && pe->v2==v2) || (pe->v1==v2 && pe->v2==v1) )
			break;
		pe = pe->next;
	}

	if (pe->next == NULL)
		return NULL;
	else
		return pe;
}

void init(graph *pg)
{
	for (int i=0; i<6; i++)
	{
		pg->vertices[i].name = i + 1;
		pg->vertices[i].incidHead = getIncident();
		pg->vertices[i].incidHead->prev = NULL;
		pg->vertices[i].incidHead->next = getIncident();
		pg->vertices[i].incidHead->next->prev = pg->vertices[i].incidHead;
		pg->vertices[i].incidHead->next->next = NULL;
	}

	pg->edgesHead = getEdge();
	pg->edgesHead->prev = NULL;
	pg->edgesHead->next = getEdge();
	pg->edgesHead->next->prev = pg->edgesHead;
	pg->edgesHead->next->next = NULL;
	
	add_edge(pg,1,2,1);
	add_edge(pg,1,3,1);
	add_edge(pg,1,4,1);
	add_edge(pg,1,6,2);
	add_edge(pg,2,3,1);
	add_edge(pg,3,5,4);
	add_edge(pg,5,5,4);
	add_edge(pg,5,6,3);
}

void printAdjacent(vertex *pv)
{
	for (incident *p=pv->incidHead->next; p->next!=NULL; p=p->next)
		printf(" %d %d",opposite(pv,p->pe),p->pe->weight);
	puts("");
}

void modifyEdge(graph *pg, int v1, int v2, int weight)
{
	edge *pe;
		
	pe = find_edge(pg->edgesHead,v1,v2);

	if (pe == NULL)
	{
		if (weight)
			add_edge(pg,v1,v2,weight);
	}
	else
	{
		if (weight)
			pe->weight = weight;
		else
			delete_edge(pg,v1,v2);
	}
}

void cleanUp(graph *pg)
{
	incident *pi, *qi;
	edge *p, *q;

	for (int j=0; j<6; j++)
		for (pi=pg->vertices[j].incidHead; pi!=NULL; pi=qi)
		{
			qi = pi->next;
			free(pi);
		}

	for (p=pg->edgesHead; p!=NULL; p=q)
	{
		q = p->next;
		free(p);
	}
}