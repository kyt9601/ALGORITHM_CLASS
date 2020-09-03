#include <stdio.h>
#include <stdlib.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

void insertion_sort(int arr[], int size);

int main()
{
	int n;
	int *arr;

	scanf("%d",&n);
	arr = (int*)malloc(sizeof(int)*n);

	for (int i=0; i<n; i++)
		scanf("%d",arr+i);

	insertion_sort(arr,n);

	for (int i=0; i<n; i++)
		printf(" %d",arr[i]);
	puts("");

	free(arr);

	return 0;
}

void insertion_sort(int arr[], int size)
{
	int i, j;
	int temp;

	for (i=1; i<size; i++)
	{
		temp = arr[i];

		for (j=i; 0<j && arr[j-1]>temp; j--)
			arr[j] = arr[j-1];

		if (i != j)
			arr[j] = temp;
	}
}