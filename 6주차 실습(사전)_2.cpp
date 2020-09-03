#include <stdio.h>
#include <stdlib.h>

int minIdxOverK(int arr[], int size, int k);

int main()
{
	int n, k;
	int *arr;

	scanf("%d %d",&n,&k);
	arr = (int*)malloc(sizeof(int)*n);

	for (int i=0; i<n; i++)
		scanf("%d",arr+i);

	printf(" %d\n",minIdxOverK(arr,n,k));

	free(arr);

	return 0;
}

int minIdxOverK(int arr[], int size, int k)
{
	int mid;
	int start, end;

	if (arr[size-1] < k)
		return size;
	else if (k <= arr[0])
		return 0;
	
	start = 0;
	end = size - 1;
	mid = (start + end) / 2;

	while (start != mid)
	{
		if (k < arr[mid])
			end = mid;
		else
			start = mid;

		mid = (start + end) / 2;
	}

	return (k<=arr[start] ? start : end);
}