#include <stdio.h>
#include <stdlib.h>

#define INFINITE 0x5fffffff

struct incident;
struct vertex;
struct edge;
struct graph;

typedef struct vertex	// 정점
{
	int name;

	int distance;
} vertex;

typedef struct edge	// 정점
{
	vertex *origin;
	vertex *dest;
	int weight;
} edge;

typedef struct graph	// 그래프
{
	vertex *verticies;
	edge *edges;
} graph;

void makeGraph(graph *pg, int n, int m);

void BellmanFordShortestPaths(graph *pg, int n, int m, int s);

void cleanUp(graph *pg);

int main()
{
	int n, m, s;
	graph g;

	scanf("%d %d %d", &n, &m, &s);
	makeGraph(&g, n, m);

	BellmanFordShortestPaths(&g, n, m, s);

	cleanUp(&g);

	return 0;
}

void makeGraph(graph *pg, int n, int m)	// 그래프 생성
{
	int v1, v2, weight;

	pg->verticies = (vertex*)malloc(sizeof(vertex) * n);
	pg->edges = (edge*)malloc(sizeof(edge) * m);

	for (int i=0; i<n; i++)	// 정점 생성과 각각의 부착 간선 리스트 초기화
		pg->verticies[i].name = i+1;

	for (int i=0; i<m; i++)	// 간선 생성과 알맞은 정점에 부착 간선 추가
	{
		scanf("%d %d %d", &v1, &v2, &weight);

		pg->edges[i].origin = pg->verticies + v1 - 1;
		pg->edges[i].dest = pg->verticies + v2 - 1;
		pg->edges[i].weight = weight;
	}
}

void BellmanFordShortestPaths(graph *pg, int n, int m, int s)
{
	for (int i=0; i<n; i++)
		pg->verticies[i].distance = INFINITE;
	pg->verticies[s-1].distance = 0;

	for (int i=1; i<n; i++)
		for (int j=0; j<m; j++)
		{
			vertex *u = pg->edges[j].origin;
			vertex *w = pg->edges[j].dest;

			if (u->distance+pg->edges[j].weight < w->distance && !(u->distance==INFINITE && w->distance==INFINITE))
				w->distance = u->distance+pg->edges[j].weight;
		}

	for (int i=0; i<n; i++)
			if (i+1 != s && pg->verticies[i].distance < INFINITE)
				printf("%d %d\n", i+1, pg->verticies[i].distance);
}

void cleanUp(graph *pg)	// 그래프에서 동적 할당된 메모리 반환
{
	free(pg->verticies);
	free(pg->edges);
}