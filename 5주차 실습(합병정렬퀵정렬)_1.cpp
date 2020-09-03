#include <stdio.h>
#include <stdlib.h>

typedef struct SinglyLinkedList
{
	int key;
	struct SinglyLinkedList *next;
} node;

typedef struct
{
	int size;
	node *start;
	node *end;
} partition;

typedef struct
{
	partition p1;
	partition p2;
} slicedPartitions;

int size;
node *head;
node *tail;

node *getNode();
void init();
void addNode(int data);
node *move(node *p, int count);
void mergeSort();
partition pMergeSort(partition par);
partition merge(partition p1, partition p2);
slicedPartitions mg_partition(partition par, int k);
void print();
void cleanUp();

int main()
{
	int data;

	init();

	scanf("%d",&size);
	for (int i=0; i<size; i++)
	{
		scanf("%d",&data);
		addNode(data);
	}

	mergeSort();

	print();
	cleanUp();

	return 0;
}

node *getNode()
{
	return (node*)malloc(sizeof(node));
}

void init()
{
	size = 0;
	head = getNode();
	tail = getNode();

	head->next = tail;
	tail->next = NULL;
}

void addNode(int data)
{
	node *nw = getNode();
	nw->key = data;

	nw->next = head->next;
	head->next = nw;
}

node *move(node *p, int count)
{
	for ( ; 0<count; count--, p=p->next);

	return p;
}

void mergeSort()
{
	partition par;

	par.size = size;
	par.start = move(head,1);
	par.end = move(head,size);

	par = pMergeSort(par);

	head->next = par.start;
	par.end->next = tail;
}

partition pMergeSort(partition par)
{
	if (par.size > 1)
	{
		slicedPartitions sPar = mg_partition(par, par.size / 2);
		sPar.p1 = pMergeSort(sPar.p1);
		sPar.p2 = pMergeSort(sPar.p2);
		return merge(sPar.p1,sPar.p2);
	}
	else
	{
		return par;
	}
}

partition merge(partition p1, partition p2)
{
	partition par;
	par.size = p1.size + p2.size;

	node *i = p1.start;
	node *j = p2.start;
	node *p;

	if (i->key < j->key)
	{
		par.start = p = i;
		i = i->next;
		(p1.size)--;
	}
	else
	{
		par.start = p = j;
		j = j->next;
		(p2.size)--;
	}

	while (p1.size && p2.size)
	{
		if (i->key < j->key)
		{
			p = p->next = i;
			i = i->next;
			(p1.size)--;
		}
		else
		{
			p = p->next = j;
			j = j->next;
			(p2.size)--;
		}
	}

	if (!p1.size)
	{
		p->next = j;
		par.end = p2.end;
	}
	else
	{
		p->next = i;
		par.end = p1.end;
	}

	return par;
}

slicedPartitions mg_partition(partition par, int k)
{
	slicedPartitions sPar;

	sPar.p1.size = k;
	sPar.p1.start = par.start;
	sPar.p1.end = move(sPar.p1.start, sPar.p1.size - 1);

	sPar.p2.size = par.size - k;
	sPar.p2.start = move(sPar.p1.end, 1);
	sPar.p2.end = move(sPar.p2.start, sPar.p2.size - 1);

	return sPar;
}

void print()
{
	for (node *p=head->next; p!=tail; p=p->next)
		printf(" %d",p->key);
	puts("");
}

void cleanUp()
{
	node *t;

	for (node *p=head; p!=NULL; p=t)
	{
		t = p->next;
		free(p);
	}
}