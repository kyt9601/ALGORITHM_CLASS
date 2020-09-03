#include <stdio.h>
#include <stdlib.h>

#define swapElements(x,y)					\
		do {								\
			edge *t = x;					\
			x = y;							\
			y = t;							\
		} while(0)

struct incident;
struct vertex;
struct edge;
struct graph;

struct prQueue;

typedef struct incident	// 부착 간선 리스트의 노드
{
	struct edge *pe;
	struct incident *next;
} incident;

typedef struct vertex	// 정점
{
	int name;
	int sack;
	incident *incidentHead;
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

typedef struct prQueue	// 우선순위 큐
{
	int n;	// 현재 큐에 저장된 원소의 개수
	edge* *elements;	// graph 의 정점들의 주소값을 저장한다
} prQueue, tree;

void makeGraph(graph *pg, int n, int m);
incident *getIncident();
void addIncident(vertex *pv, edge *pe);
vertex *opposite(vertex *pv, edge *pe);

void KruskalMST(graph *pg, int n, int m);
int isEmpty(prQueue *pQueue);
void initQueue(prQueue *pQueue, graph *pg, int m);
void insertElement(prQueue *pQueue, edge *element);
edge *removeMin(prQueue *pQueue);
void upHeap(prQueue *pQueue, int idx);
void downHeap(prQueue *pQueue, int idx);
void cleanQueue(prQueue *pQueue);

void cleanUp(graph *pg, int n);

int main()
{
	int n, m;
	graph g;

	scanf("%d %d", &n, &m);
	makeGraph(&g, n, m);

	KruskalMST(&g, n, m);

	cleanUp(&g, n);

	return 0;
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
	}

	for (int i=0; i<m; i++)	// 간선 생성과 알맞은 정점에 부착 간선 추가
	{
		scanf("%d %d %d", &v1, &v2, &weight);

		pg->edges[i].pv1 = pg->verticies + v1 - 1;
		pg->edges[i].pv2 = pg->verticies + v2 - 1;
		pg->edges[i].weight = weight;

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
	incident *nw = getIncident();
	nw->pe = pe;

	nw->next = pv->incidentHead->next;
	pv->incidentHead->next = nw;
}

vertex *opposite(vertex *pv, edge *pe)	// 정점과 간선이 주어질때, 반대편 정점 반환
{
	if (pe->pv1->name == pv->name)
		return pe->pv2;
	else
		return pe->pv1;	
}

void KruskalMST(graph *pg, int n, int m)	// Prim-Jarnik 알고리즘
{
	prQueue queue;	// 우선순위 큐
	tree T;
	int minCost;

	for (int i=0; i<n; i++)
		pg->verticies[i].sack = i + 1;

	initQueue(&queue, pg, m);	// 우선순위 큐에 원소들 삽입과 초기화
	
	T.n = 0;
	T.elements = (edge**)malloc(sizeof(edge*) * m);

	minCost = 0;

	while (T.n < n-1)
	{
		edge *pe = removeMin(&queue);

		if (pe->pv1->sack != pe->pv2->sack)
		{
			printf(" %d", pe->weight);
			minCost += pe->weight;

			T.elements[T.n++] = pe;

			int sack1 = pe->pv1->sack;
			int sack2 = pe->pv2->sack;
			for (int i=0; i<n; i++)
				if (pg->verticies[i].sack == sack2)
					pg->verticies[i].sack = sack1;
		}
	}
	
	printf("\n%d\n", minCost);

	cleanQueue(&queue);
}

int isEmpty(prQueue *pQueue)	// 큐가 비었는지 확인
{
	return !(pQueue->n);
}

void initQueue(prQueue *pQueue, graph *pg, int m)	// 우선순위 큐에 원소 삽입 및 초기화
{
	pQueue->elements = (edge**)malloc(sizeof(edge*) * (m+1));
	pQueue->n = 0;

	for (int i=0; i<m; i++)
		insertElement(pQueue, pg->edges + i);
}

void insertElement(prQueue *pQueue, edge *element)	// 우선순위 큐에 원소 삽입
{
	pQueue->elements[++(pQueue->n)] = element;
	upHeap(pQueue, pQueue->n);
}

edge *removeMin(prQueue *pQueue)	// 우선순위 큐 내에서의 최소값 반환
{
	edge *min = pQueue->elements[1];

	pQueue->elements[1] = pQueue->elements[(pQueue->n)--];
	downHeap(pQueue, 1);

	return min;
}

void upHeap(prQueue *pQueue, int idx)	// 힙 속성 복구 upHeap
{
	int parent = idx / 2;

	if (!parent)
		return;

	if (pQueue->elements[parent]->weight > pQueue->elements[idx]->weight)
	{
		swapElements(pQueue->elements[parent], pQueue->elements[idx]);
		upHeap(pQueue, parent);
	}
}

void downHeap(prQueue *pQueue, int idx)	// 힙 속성 복구 downHeap
{
	int left = idx * 2;
	int right = idx * 2 + 1;
	int smaller;

	if (pQueue->n < left)
		return;
	else
		smaller = left;

	if (right <= pQueue->n)
		if (pQueue->elements[left]->weight > pQueue->elements[right]->weight)
			smaller = right;

	if (pQueue->elements[idx]->weight > pQueue->elements[smaller]->weight)
	{
		swapElements(pQueue->elements[idx], pQueue->elements[smaller]);
		downHeap(pQueue, smaller);
	}
}

void cleanQueue(prQueue *pQueue)	// 우선순위 큐에서 동적 할당된 메모리 반환
{
	free(pQueue->elements);
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