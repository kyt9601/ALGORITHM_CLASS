#include <stdio.h>
#include <stdlib.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

void buildHeap(int H[], int last, int key);
void upHeap(int H[], int idx);

int main()
{
    int n;
    int key;
    int *H;

    scanf("%d",&n);
    H = (int*)malloc(sizeof(int)*(n+1));

    for (int j=1; j<=n; j++)
    {
	    scanf("%d",&key);
	    buildHeap(H,j,key);
    }

    for (int j=1; j<=n; j++)
	    printf(" %d",H[j]);
    puts("");

    free(H);

    return 0;
}

void buildHeap(int H[], int last, int key)
{
    H[last] = key;
    upHeap(H,last);
}

void upHeap(int H[], int idx)
{
    int parent = idx / 2;

    if (!parent)
	    return;

    if (H[parent] < H[idx])
    {
	    swap(H[parent],H[idx]);
	    upHeap(H,parent);
    }
}