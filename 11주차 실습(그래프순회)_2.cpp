#include <stdio.h>
#include <stdlib.h>

struct incident;
struct vertex;
struct edge;
struct graph;

typedef enum
{
	FRESH, VISITED
} status;

typedef struct incident	// 부착 간선 리스트의 노드
{
	struct edge *pe;
	struct incident *next;
} incident;

typedef struct vertex	// 정점
{
	int name;
	incident *incidentHead;
	status st;
} vertex;

typedef struct edge	// 정점
{
	vertex *pv1;
	vertex *pv2;
	int weight;
} edge;

typedef struct graph	// 그래프
{
	vertex *verticies;
	edge *edges;
} graph;

typedef struct node
{
	vertex *pv;
	struct node *prev;
	struct node *next;
} node;

typedef struct
{
	node *head;
	node *tail;
} queue;

void makeGraph(graph *pg, int n, int m);
incident *getIncident();
void addIncident(vertex *pv, edge *pe);
vertex *opposite(vertex *pv, edge *pe);

void cleanUp(graph *pg, int n);

void BFS(graph *pg, int s);

int main()
{
	int n, m, s;
	graph g;

	scanf("%d %d %d", &n, &m, &s);
	makeGraph(&g, n, m);

	BFS(&g, s);

	cleanUp(&g, n);

	return 0;
}

node *getNode()
{
	return (node*)malloc(sizeof(node));
}

void enqueue(queue *pQ, vertex *pv)
{
	node *nw = getNode();
	nw->pv = pv;

	nw->next = pQ->tail->prev->next;
	nw->prev = pQ->tail->prev;

	pQ->tail->prev->next = nw;
	pQ->tail->prev = nw;
}

vertex *dequeue(queue *pQ)
{
	node *p = pQ->head->next;
	vertex *pv = p->pv;

	p->prev->next = p->next;
	p->next->prev = p->prev;

	free(p);
	return pv;
}

int isEmpty(queue *pQ)
{
	return (pQ->head->next == pQ->tail);
}

void BFS(graph *pg, int s)
{
	queue Q;

	Q.head = getNode();
	Q.head->prev = NULL;
	Q.head->next = Q.tail = getNode();
	Q.tail->prev = Q.head;
	Q.tail->next = NULL;

	enqueue(&Q, pg->verticies + s - 1);
	pg->verticies[s-1].st = VISITED;

	while (!isEmpty(&Q))
	{
		vertex *u = dequeue(&Q);
		printf("%d\n", u->name);

		for (incident *p=u->incidentHead->next; p!=NULL; p=p->next)
		{
			vertex *op = opposite(u, p->pe);

			if (op->st == FRESH)
			{
				enqueue(&Q, op);
				op->st = VISITED;
			}
		}
	}
}

void makeGraph(graph *pg, int n, int m)	// 그래프 생성
{
	int v1, v2, weight;

	pg->verticies = (vertex*)malloc(sizeof(vertex) * n);
	pg->edges = (edge*)malloc(sizeof(edge) * m);

	for (int i=0; i<n; i++)	// 정점 생성과 각각의 부착 간선 리스트 초기화
	{
		pg->verticies[i].name = i+1;
		pg->verticies[i].incidentHead = getIncident();
		pg->verticies[i].incidentHead->next = NULL;
		pg->verticies[i].st = FRESH;
	}

	for (int i=0; i<m; i++)	// 간선 생성과 알맞은 정점에 부착 간선 추가
	{
		scanf("%d %d", &v1, &v2);

		pg->edges[i].pv1 = pg->verticies + v1 - 1;
		pg->edges[i].pv2 = pg->verticies + v2 - 1;
		pg->edges[i].weight = 0;

		addIncident(pg->verticies + v1 - 1, pg->edges + i);
		if (v1 != v2)
			addIncident(pg->verticies + v2 - 1, pg->edges + i);
	}
}

incident *getIncident()	// 부착 간선 리스트 노드 동적 할당
{
	return (incident*)malloc(sizeof(incident));
}

void addIncident(vertex *pv, edge *pe)	// 정점의 부착 간선 리스트에 부착 간선 추가
{
	incident *p, *q;
	incident *nw = getIncident();
	nw->pe = pe;

	q = pv->incidentHead;
	for (p=q->next; p!=NULL && opposite(pv,p->pe)->name<opposite(pv,pe)->name; p=q->next)
		q = q->next;

	nw->next = q->next;
	q->next = nw;
}

vertex *opposite(vertex *pv, edge *pe)	// 정점과 간선이 주어질때, 반대편 정점 반환
{
	if (pe->pv1->name == pv->name)
		return pe->pv2;
	else
		return pe->pv1;	
}

void cleanUp(graph *pg, int n)	// 그래프에서 동적 할당된 메모리 반환
{
	incident *pi, *qi;

	for (int i=0; i<n; i++) {
		for (pi=pg->verticies[i].incidentHead; pi!=NULL; pi=qi) {
			qi = pi->next;
			free(pi);
		}
	}

	free(pg->verticies);
	free(pg->edges);
}