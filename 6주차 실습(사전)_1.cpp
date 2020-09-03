#include <stdio.h>
#include <stdlib.h>

int maxIdxUnderK(int arr[], int size, int k);
int rMaxIdxUnderK(int arr[], int start, int end, int k);

int main()
{
	int n, k;
	int *arr;

	scanf("%d %d",&n,&k);
	arr = (int*)malloc(sizeof(int)*n);

	for (int i=0; i<n; i++)
		scanf("%d",arr+i);

	printf(" %d\n",maxIdxUnderK(arr,n,k));

	free(arr);

	return 0;
}

int maxIdxUnderK(int arr[], int size, int k)
{
	if (k < arr[0])
		return -1;
	else if (arr[size-1] <= k)
		return size - 1;
	else
		return rMaxIdxUnderK(arr,0,size-1,k);
}

int rMaxIdxUnderK(int arr[], int start, int end, int k)
{
	int mid = (start + end) / 2;

	if (start == mid)
		return (arr[end]<=k ? end : start);
	else if (k < arr[mid])
		return rMaxIdxUnderK(arr,start,mid,k);
	else
		return rMaxIdxUnderK(arr,mid,end,k);
}