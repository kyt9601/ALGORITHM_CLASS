#include <stdio.h>
#include <stdlib.h>

typedef struct edge
{
	int origin;
	int destination;
} edge;

typedef struct ioedge
{
	int element;
	struct ioedge *next;
} ioedge;

typedef struct
{
	char name;
	ioedge *outEdges;
	ioedge *inEdges;
	int inDegree;
} vertex;

typedef struct
{
	vertex vertices[99];
	edge edges[999];
} graph;

typedef struct QNode
{
	int idx;
	struct QNode *prev;
	struct QNode *next;
} QNode;

graph G;
int n, m;
int topOrder[100];
QNode *QHead, *QTail;

ioedge *getIOEdge();
void buildGraph();
int index(char vName);
void insertVertex(char vName, int i);
void insertDirectedEdge(char uName, char wName, int i);
void insertFirst(ioedge *head, int i);

QNode *getQNode();
void initQueue();
void cleanQueue();
int isQueueEmpty();
void enqueue(int w);
int dequeue();

void topologicalSort();
void cleanUp();

int main()
{
	buildGraph();

	topologicalSort();

	if (topOrder[0] == 0)
		puts("0");
	else
		for (int i=1; i<=n; i++) {
			putchar(G.vertices[topOrder[i]].name);
			putchar(' ');
		}
	
	cleanUp();

	return 0;
}

ioedge *getIOEdge()
{
	return (ioedge*)malloc(sizeof(ioedge));
}

void buildGraph()
{
	char vName;
	char uName, wName;

	scanf("%d",&n); getchar();

	for (int i=0; i<n; i++)
	{
	    scanf("%c",&vName); getchar();
		insertVertex(vName,i);
	}

	scanf("%d",&m); getchar();

	for (int i=0; i<m; i++)
	{
	    scanf("%c %c",&uName,&wName); getchar();
		insertDirectedEdge(uName,wName,i);
	}	
}

int index(char vName)
{
	for (int i=0; i<n; i++)
		if (G.vertices[i].name == vName)
			return i;
}

void insertVertex(char vName, int i)
{
	G.vertices[i].name = vName;

	G.vertices[i].outEdges = getIOEdge();
	G.vertices[i].outEdges->next = NULL;

	G.vertices[i].inEdges = getIOEdge();
	G.vertices[i].inEdges->next = NULL;

	G.vertices[i].inDegree = 0;
}

void insertDirectedEdge(char uName, char wName, int i)
{
	int u, w;

	u = index(uName);
	w = index(wName);

	G.edges[i].origin = u;
	G.edges[i].destination = w;

	insertFirst(G.vertices[u].outEdges,i);
	insertFirst(G.vertices[w].inEdges,i);

	G.vertices[w].inDegree++;
}

void insertFirst(ioedge *head, int i)
{
	ioedge *nw = getIOEdge();
	nw->element = i;
	nw->next = head->next;
	head->next = nw;
}

QNode *getQNode()
{
	return (QNode*)malloc(sizeof(QNode));
}

void initQueue()
{
	QHead = getQNode();
	QTail = getQNode();

	QHead->prev = NULL;
	QHead->next = QTail;
	QTail->prev = QHead;
	QTail->next = NULL;
}

void cleanQueue()
{
	free(QHead);
	free(QTail);
}

int isQueueEmpty()
{
	return (QHead->next == QTail);
}

void enqueue(int w)
{
	QNode *nw = getQNode();
	nw->idx = w;

	nw->next = QTail;
	nw->prev = QTail->prev;
	QTail->prev->next = nw;
	QTail->prev = nw;
}

int dequeue()
{
	QNode *p = QHead->next;
	int idx = p->idx;

	p->prev->next = p->next;
	p->next->prev = p->prev;
	free(p);

	return idx;
}

void topologicalSort()
{
	int t;
	int in[99];

	initQueue();

	for (int i=0; i<n; i++)
	{
		in[i] = G.vertices[i].inDegree;
		if (in[i] == 0)
			enqueue(i);
	}

	t = 1;

	while (!isQueueEmpty())
	{
		int u = dequeue();
		topOrder[t] = u;
		t++;

		for (ioedge *p=G.vertices[u].outEdges->next; p!=NULL; p=p->next)
		{
			int w = G.edges[p->element].destination;
			in[w] = in[w] - 1;
			if (in[w] == 0)
				enqueue(w);
		}
	}

	cleanQueue();

	if (t <= n)
		topOrder[0] = 0;
	else
		topOrder[0] = 1;
}

void cleanUp()
{
	ioedge *p, *q;

	for (int i=0; i<n; i++)
	{
		for (p=G.vertices[i].outEdges; p!=NULL; p=q) {
			q = p->next;
			free(p);
		}

		for (p=G.vertices[i].inEdges; p!=NULL; p=q) {
			q = p->next;
			free(p);
		}
	}
}