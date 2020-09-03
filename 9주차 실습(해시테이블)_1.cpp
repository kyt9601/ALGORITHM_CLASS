#include <stdio.h>
#include <stdlib.h>

typedef struct SinglyLinkedList
{
	int key;
	struct SinglyLinkedList *next;
} bucket;

int hash(int x, int M);
void init(bucket ***pHashTable, int M);
bucket *getBucket();
void insert(bucket *hashTable[], int M, int x);
void _delete(bucket *hashTable[], int M, int x);
void search(bucket *hashTable[], int M, int x);
void print(bucket *hashTable[], int M);
void cleanUp(bucket *hashTable[], int M);

int main()
{
	int M;
	char op;
	bucket **hashTable;
	char tmp;

	scanf("%d",&M); tmp = getchar();
	if (tmp == ' ') getchar();
	init(&hashTable,M);
	
	while ((op = getchar()) != 'e')
	{
		int x;
		
		switch (op)
		{
		case 'i' : scanf("%d",&x); insert(hashTable,M,x); break;
		case 'd' : scanf("%d",&x); _delete(hashTable,M,x); break;
		case 's' : scanf("%d",&x); search(hashTable,M,x); break;
		case 'p' : print(hashTable,M); break;
		}
		
		getchar();
	}
	
	cleanUp(hashTable,M);
	
	return 0;
}

int hash(int x, int M)
{
	return (x % M);
}

void init(bucket ***pHashTable, int M)
{
	*pHashTable = (bucket**)malloc(sizeof(bucket*)*M);
	for (int i=0; i<M; i++)
	{
		(*pHashTable)[i] = getBucket();
		(*pHashTable)[i]->next = NULL;
	}
}

bucket *getBucket()
{
	return (bucket*)malloc(sizeof(bucket));
}

void insert(bucket *hashTable[], int M, int x)
{
	int H = hash(x,M);
	bucket *nw = getBucket();

	nw->key = x;
	nw->next = hashTable[H]->next;
	hashTable[H]->next = nw;
}

void _delete(bucket *hashTable[], int M, int x)
{
	int H = hash(x,M);
	int count;
	bucket *p, *q;

	count = 1;
	p = hashTable[H];
	q = p->next;
	while (q!=NULL && q->key!=x)
	{
		p = p->next;
		q = q->next;
		count++;
	}
	
	if (q != NULL)
	{
		printf("%d\n",count);
		p->next = q->next;
		free(q);
	}
	else
	{
		puts("0");
	}
}

void search(bucket *hashTable[], int M, int x)
{
	int H = hash(x,M);
	int count;
	bucket *p;

	count = 1;
	for (p=hashTable[H]->next; p!=NULL && p->key!=x; p=p->next, count++);
	
	if (p != NULL)
		printf("%d\n",count);
	else
		puts("0");
}

void print(bucket *hashTable[], int M)
{
	int empty = 1;
	for (int i=0; i<M; i++)
		for (bucket *p=hashTable[i]->next; p!=NULL; p=p->next)
		{
			printf(" %d",p->key);
			empty = 0;
		}

	if (!empty)
		puts("");
}

void cleanUp(bucket *hashTable[], int M)
{
	for (int i=0; i<M; i++)
	{
		bucket *t;
		for (bucket *p=hashTable[i]; p!=NULL; p=t)
		{
			t = p->next;
			free(p);
		}
	}
	
	free(hashTable);
}
/*
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
}*/
/*
#include <stdio.h>
#include <stdlib.h>

int hash1(int x, int M);
int hash2(int H, int M);
int range(int idx, int M);
int firstEmptyBucket(int hashTable[], int M, int bucket);
void insert(int hashTable[], int M, int x);
void search(int hashTable[], int M, int x);

int main()
{
	int M, N, q;
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

int hash1(int x, int M)
{
	return (x % M);
}

int hash2(int x, int q)
{
	return (q - (x % q));
}

int range(int idx, int M)
{
	return (idx + M) % M;
}

int firstEmptyBucket(int hashTable[], int M, int bucket)
{
	int origin = bucket;
	int count;

	count = 1;
	while (hashTable[bucket] != 0)
	{
		printf("C");
		bucket = range(origin + hash2();
	}
	return bucket;
}

void insert(int hahTsable[], int M, int x)
{
	int H = hash1(x,M);
	int idx = firstEmptyBucket(hashTable,M,H);
	
	hashTable[idx] = x;
	printf("%d\n",idx);
}

void search(int hashTable[], int M, int x)
{
	int H = hash1(x,M);
	int idx;
	int count;

	count = 0;
	for (idx=H; count<M && hashTable[idx]!=x; idx=range(idx+1,M), count++);
	
	if (count == M)
		puts("-1");
	else
		printf("%d %d\n",idx,hashTable[idx]);
}*/