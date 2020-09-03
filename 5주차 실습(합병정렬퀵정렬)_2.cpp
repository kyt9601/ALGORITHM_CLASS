#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

typedef struct
{
	int start;
	int end;
} partition;

typedef struct
{
	partition less;
	partition greater;
} slicedPartitions;

void quickSort(int arr[], int size);
void inPlaceQuickSort(int arr[], partition par);
slicedPartitions inPlacePartitions(int arr[], partition par, int k);
int find_pivot_index(int arr[], partition par);
void print(int arr[], int size);

int main()
{
	int n;
	int *arr;

	scanf("%d",&n);
	arr = (int*)malloc(sizeof(int)*n);

	for (int i=0; i<n; i++)
		scanf("%d",arr+i);

	quickSort(arr,n);

	print(arr,n);
	free(arr);

	return 0;
}

void quickSort(int arr[], int size)
{
	partition par;

	par.start = 0;
	par.end = size-1;

	inPlaceQuickSort(arr,par);
}

void inPlaceQuickSort(int arr[], partition par)
{
	int k;
	slicedPartitions sPar;

	if (par.end <= par.start)
		return;

	k = find_pivot_index(arr,par);
	sPar = inPlacePartitions(arr,par,k);
	inPlaceQuickSort(arr,sPar.less);
	inPlaceQuickSort(arr,sPar.greater);
}

slicedPartitions inPlacePartitions(int arr[], partition par, int k)
{
	int i, j;
	int p;
	int kCount;
	int kPrev;
	slicedPartitions sPar;

	sPar.less.start = par.start;
	sPar.greater.end = par.end;

    p = arr[k];
	for (kPrev=par.end; arr[kPrev]==p; kPrev--);
	
	for (int h=par.start; h<kPrev; h++)
		if (arr[h] == p)
		{
			swap(arr[h],arr[kPrev]);
			
			for ( ; arr[kPrev]==p; kPrev--);
		}
		
	kCount = par.end - kPrev;

	i = par.start;
	j = kPrev;

	while (i <= j)
	{
		while (i<=j && arr[i]<p)
			i++;

		while (i<=j && arr[j]>p)
			j--;

		if (i < j)
			swap(arr[i],arr[j]);
	}

	for (int h=0; h<kCount; h++)
		swap(arr[i+h],arr[kPrev+1+h]);

	sPar.less.end = i - 1;
	sPar.greater.start = i + kCount;

	return sPar;
}

int find_pivot_index(int arr[], partition par)
{
	int tmpIdx[3];

	srand(time(NULL));

	for (int i=0; i<3; i++)
		tmpIdx[i] = rand() % (par.end-par.start+1) + par.start;

	for (int i=1; i<3; i++)
	{
	    int j;
	    int t = tmpIdx[i];
	    
	    for (j=i-1; 0<=j && arr[t]<arr[tmpIdx[j]]; j--)
	        tmpIdx[j+1] = tmpIdx[j];
	        
        tmpIdx[j+1] = t;
	}
	
	return tmpIdx[1];
}

void print(int arr[], int size)
{
	for (int i=0; i<size; i++)
		printf(" %d",arr[i]);
	puts("");
}