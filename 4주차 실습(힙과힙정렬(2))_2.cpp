#include <stdio.h>
#include <stdlib.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

int *H;
int n, N;

void rBuildHeap(int subRoot);
void downHeap(int idx);
void inPlaceHeapSort();
// void rInPlaceHeapSort();
void printArray();

int main()
{
	scanf("%d",&N);
	H = (int*)malloc(sizeof(int)*(N+1));
	n = N;

	for (int i=1; i<=N; i++)
		scanf("%d",H+i);

	inPlaceHeapSort();

	printArray();
	free(H);

	return 0;
}

void rBuildHeap(int subRoot)
{
	if (N < subRoot)
		return;
	
	rBuildHeap(subRoot * 2);
	rBuildHeap(subRoot * 2 + 1);
	downHeap(subRoot);
}

void downHeap(int idx)
{
	int left = idx * 2;
	int right = idx * 2 + 1;
	int greater;

	if (n < left)
		return;
	else
		greater = left;
	
	if (right <= n)
		if (H[left] < H[right])
			greater = right;
	
	if (H[idx] < H[greater])
	{
		swap(H[idx],H[greater]);
		downHeap(greater);
	}
}

void inPlaceHeapSort()
{
	rBuildHeap(1);
	
	while (1 < n)
	{
		swap(H[1],H[n]);
		n--;
		downHeap(1);
	}
}
/*
void inPlaceHeapSort()
{
	rBuildHeap(1);
	rInPlaceHeapSort();
}

void rInPlaceHeapSort()
{
	if (n <= 1)
		return;

	swap(H[1],H[n]);
	n--;

	downHeap(1);
	inPlaceHeapSort();	
}
*/
void printArray()
{
	for (int i=1; i<=N; i++)
		printf(" %d",H[i]);
	puts("");
}