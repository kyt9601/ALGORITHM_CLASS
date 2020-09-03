#include <stdio.h>
#include <stdlib.h>

#define INFINITE 0x5fffffff	// 무한대 정의

// 최소힙에서 up/downHeap 시 사용되는 swap
// 임시 변수 t 를 이용해 바꾼후, 우선순위 큐 내에서의 인덱스 번호를 알맞게 갱신
// idxPriorityQueue 를 관리하는 이유는 167번째 줄에서 반대편 정점을 찾고,
// replaceKey 에서 upHeap 을 할때 인덱스를 상수 시간에 구하기 위함.
#define swapElements(x,y)					\
		do {								\
			int idx1 = x->idxPriorityQueue;	\
			int idx2 = y->idxPriorityQueue;	\
			vertex *t = x;					\
			x = y;							\
			y = t;							\
			x->idxPriorityQueue = idx1;		\
			y->idxPriorityQueue = idx2;		\
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
	incident *incidentHead;

	int distance;
	int inPriorityQueue;	// 우선순위 큐 내에 포함되어있는지 확인하는 멤버
	int idxPriorityQueue;	// 정점을 통해 우선순위 큐 내에서의 인덱스를 상수 시간에 구하기 위함.
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
	vertex* *elements;	// graph 의 정점들의 주소값을 저장한다
} prQueue;

void makeGraph(graph *pg, int n, int m);
incident *getIncident();
void addIncident(vertex *pv, edge *pe);
vertex *opposite(vertex *pv, edge *pe);

void PrimJarnikMST(graph *pg, int n);
int isEmpty(prQueue *pQueue);
void initQueue(prQueue *pQueue, graph *pg, int n);
void insertElement(prQueue *pQueue, vertex *element);
vertex *removeMin(prQueue *pQueue);
void replaceKey(prQueue *pQueue, vertex *element, int distance);
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

	PrimJarnikMST(&g, n);

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

void PrimJarnikMST(graph *pg, int n)	// Prim-Jarnik 알고리즘
{
	vertex *s;
	prQueue queue;	// 우선순위 큐
	int minCost = 0;	// 최소 비용

	for (int i=1; i<n; i++)
		pg->verticies[i].distance = INFINITE;

	s = pg->verticies;	// 1번 정점
	s->distance = 0;

	initQueue(&queue, pg, n);	// 우선순위 큐에 원소들 삽입과 초기화

	while (!isEmpty(&queue))
	{
		vertex *u = removeMin(&queue);
		printf(" %d", u->name);	// 배낭에 들어간 원소 출력 (실제 배낭에 넣지는 않는다)
		minCost += u->distance;

		for (incident *pi=u->incidentHead->next; pi!=NULL; pi=pi->next)
		{
			vertex *z = opposite(u, pi->pe);

			if (z->inPriorityQueue && pi->pe->weight < z->distance)
				replaceKey(&queue, z, pi->pe->weight);	// z의 idxPriorityQueue 멤버를 통해 우선순위 큐 내에서의 인덱스를 상수 시간에 확인 가능하다.
		}
	}
	puts("");

	printf("%d\n", minCost);

	cleanQueue(&queue);
}

int isEmpty(prQueue *pQueue)	// 큐가 비었는지 확인
{
	return !(pQueue->n);
}

void initQueue(prQueue *pQueue, graph *pg, int n)	// 우선순위 큐에 원소 삽입 및 초기화
{
	pQueue->elements = (vertex**)malloc(sizeof(vertex*) * (n+1));
	pQueue->n = 0;

	for (int i=0; i<n; i++)
	{
		pg->verticies[i].inPriorityQueue = 1;	// 우선순위 큐 내에 존재함
		pg->verticies[i].idxPriorityQueue = i+1;	// 우선순위 큐 내에서의 인덱스

		insertElement(pQueue, pg->verticies + i);
	}
}

void insertElement(prQueue *pQueue, vertex *element)	// 우선순위 큐에 원소 삽입
{
	pQueue->elements[++(pQueue->n)] = element;
	upHeap(pQueue, pQueue->n);
}

vertex *removeMin(prQueue *pQueue)	// 우선순위 큐 내에서의 최소값 반환
{
	vertex *min = pQueue->elements[1];
	min->inPriorityQueue = 0;	// 우선순위 큐에서 빠짐

	pQueue->elements[1] = pQueue->elements[(pQueue->n)--];
	pQueue->elements[1]->idxPriorityQueue = 1;	// 마지막에서 처음으로 가져온 원소의 인덱스 갱신
	downHeap(pQueue, 1);

	return min;
}

void replaceKey(prQueue *pQueue, vertex *element, int distance)	// 원소값 갱신과 힙 속성 복구
{
	element->distance = distance;
	upHeap(pQueue, element->idxPriorityQueue);	// 정점을 통해 우선순위 큐 내에서의 인덱스 확인 가능
}

void upHeap(prQueue *pQueue, int idx)	// 힙 속성 복구 upHeap
{
	int parent = idx / 2;

	if (!parent)
		return;

	if (pQueue->elements[parent]->distance > pQueue->elements[idx]->distance)
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
		if (pQueue->elements[left]->distance > pQueue->elements[right]->distance)
			smaller = right;

	if (pQueue->elements[idx]->distance > pQueue->elements[smaller]->distance)
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