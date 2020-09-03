#include <stdio.h>
#include <stdlib.h>

int hash(int x, int M);
int range(int idx, int M);
int firstEmptyBucket(int hashTable[], int M, int bucket);
void insert(int hashTable[], int M, int x);
void search(int hashTable[], int M, int x);

int main()
{
	int M, N;
	int count;
	int *hashTable;
	char op;
	
	scanf("%d %d",&M,&N); getchar();
	hashTable = (int*)calloc(M,sizeof(int));
	
	count = 0;
	while ((op = getchar()) != 'e')
	{
		int x;
		scanf("%d",&x); getchar();
		
		if (op == 'i')
		{
			if (N <= count)
				continue;
			count++;
			insert(hashTable,M,x);
		}
		else
		{
			search(hashTable,M,x);
		}
	}
	
	free(hashTable);
	
	return 0;
}

int hash(int x, int M)
{
	return (x % M);
}

int range(int idx, int M)
{
	return (idx + M) % M;
}

int firstEmptyBucket(int hashTable[], int M, int bucket)
{
	while (hashTable[bucket] != 0)
	{
		printf("C");
		bucket = range(bucket+1,M);
	}
	return bucket;
}

void insert(int hashTable[], int M, int x)
{
	int H = hash(x,M);
	int idx = firstEmptyBucket(hashTable,M,H);
	
	hashTable[idx] = x;
	printf("%d\n",idx);
}

void search(int hashTable[], int M, int x)
{
	int H = hash(x,M);
	int idx;
	int count;

	count = 0;
	for (idx=H; count<M && hashTable[idx]!=x; idx=range(idx+1,M), count++);
	
	if (count == M)
		puts("-1");
	else
		printf("%d %d\n",idx,hashTable[idx]);
}