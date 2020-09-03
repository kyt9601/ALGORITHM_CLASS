#include <stdio.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

int n;
int H[100];

void insertItem(int key);
int removeMax();
void upHeap(int idx);
void downHeap(int idx);
void printHeap();

int main()
{
	char op;
	int key;
	
	while ( (op = getchar()) != 'q')
	{
		switch (op)
		{
			case 'i' : scanf("%d",&key); insertItem(key); puts("0"); break;
			case 'd' : printf("%d\n",removeMax()); break;
			case 'p' : printHeap(); break;
		}
		getchar();
	}
	
	return 0;
}

void insertItem(int key)
{
	H[++n] = key;
	upHeap(n);
}

int removeMax()
{
	int key = H[1];
	H[1] = H[n--];
	downHeap(1);
	return key;
}

void upHeap(int idx)
{
	int parent = idx / 2;
	
	if (!parent)
		return;
	
	if (H[parent] < H[idx])
	{
		swap(H[parent],H[idx]);
		upHeap(parent);
	}
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