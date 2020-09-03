#include <stdio.h>
#include <stdlib.h>

#define swap(x,y) do{int t=x; x=y; y=t;}while(0)

typedef struct
{
    int start;
    int end;
} partition;

typedef struct
{
    partition l;
    partition g;
} dividedPartitions;

void quickSort(int arr[], int n);
void _quickSort(int arr[], partition par);
dividedPartitions partitioning(int arr[], partition par, int pivot);

int main()
{
    int n;
    int *arr;

    scanf("%d",&n);
    arr = (int*)malloc(sizeof(int)*n);

    for (int i=0; i<n; i++)
	    scanf("%d",arr+i);

    quickSort(arr,n);

    for (int i=0; i<n; i++)
	    printf(" %d",arr[i]);
    puts("");

    free(arr);

    return 0;
}

void quickSort(int arr[], int n)
{
    partition par;

    par.start = 0;
    par.end = n-1;

    _quickSort(arr,par);
}

void _quickSort(int arr[], partition par)
{
    if (par.end <= par.start)
    {
	    return;
    }
    else
    {
	    int pivot = (par.start + par.end) / 2;
	    dividedPartitions dPars = partitioning(arr,par,pivot);

	    _quickSort(arr,dPars.l);
	    _quickSort(arr,dPars.g);
    }
}

dividedPartitions partitioning(int arr[], partition par, int pivot)
{
    int i, j;
    int k = arr[pivot];
    int kCount;
    int kPrev;
    dividedPartitions dPars;
    dPars.l.start = par.start;
    dPars.g.end = par.end;

    for (kPrev=par.end; arr[kPrev]==k; kPrev--);

    for (int h=par.start; h<kPrev; h++)
    {
	    if (arr[h] == k)
	        swap(arr[h],arr[kPrev]);

	    for ( ; arr[kPrev]==k; kPrev--);
    }

    kCount = par.end - kPrev;

    i = par.start;
    j = kPrev;
    while (i<=j)
    {
	    while (i<=j && k>arr[i])
	        i++;
	    while (i<=j && k<arr[j])
	        j--;

	    if (i<j)
	        swap(arr[i],arr[j]);
    }

    dPars.l.end = i-1;
    dPars.g.start = i + kCount;

    for (int h=0; h<kCount; h++)
	    swap(arr[i+h],arr[kPrev+h+1]);

    return dPars;
}