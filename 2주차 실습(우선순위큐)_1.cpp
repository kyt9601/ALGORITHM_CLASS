#include <stdio.h>
#include <stdlib.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

void selection_sort(int arr[], int size);

int main()
{
	int n;
	int *arr;

	scanf("%d",&n);
	arr = (int*)malloc(sizeof(int)*n);

	for (int i=0; i<n; i++)
		scanf("%d",arr+i);

	selection_sort(arr,n);

	for (int i=0; i<n; i++)
		printf(" %d",arr[i]);
	puts("");

    free(arr);

	return 0;
}

void selection_sort(int arr[], int size)
{
	int maxIdx;

	for (int i=size-1; 0<i; i--)
	{
		maxIdx = i;

		for (int j=i-1; 0<=j; j--)
			if (arr[maxIdx] < arr[j])
				maxIdx = j;

		if (i != maxIdx)
			swap(arr[i],arr[maxIdx]);
	}
}