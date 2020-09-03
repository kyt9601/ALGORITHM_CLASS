#include <stdio.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

int n;
int H[100];

void rBuildHeap(int subRoot);
void downHeap(int idx);
void printHeap();

int main()
{
	scanf("%d",&n);
	
	for (int i=1; i<=n; i++)
		scanf("%d",H+i);
	
	rBuildHeap(1);
	printHeap();
	
	return 0;
}

void rBuildHeap(int subRoot)
{
	if (n < subRoot)
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

void printHeap()
{
	for (int i=1; i<=n; i++)
		printf(" %d",H[i]);
	puts("");
}