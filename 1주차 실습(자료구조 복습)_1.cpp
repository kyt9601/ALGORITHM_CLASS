#include <stdio.h>
#include <stdlib.h>
 
typedef struct DoublyLinkedList
{
	char elem;
	struct DoublyLinkedList *prev;
	struct DoublyLinkedList *next;
} node;
 
typedef struct
{
	node *head;
	node *tail;
} list;
 
node *getNode();
void init(list *pList);
void add(list List, int position, char item);
void _delete(list List, int position);
char get_entry(list List, int position);
void print(list List);
void cleanUp(list List);
void errorHandling();
 
int main()
{
	int n, pos;
	char op, item;
	char ch;
	list List;
 
	init(&List);
 
	scanf("%d",&n);
	for (int i=0; i<n; i++)
	{
		getchar();
		scanf("%c",&op);
 
		switch(op)
		{
		case 'A' : scanf("%d %c",&pos,&item); add(List,pos,item); break;
		case 'D' : scanf("%d",&pos); _delete(List,pos); break;
		case 'G' : scanf("%d",&pos); if ( (ch = get_entry(List,pos)) ) { putchar(ch); puts(""); } break;
		case 'P' : print(List); break;
		}
	}
 
	cleanUp(List);
 
	return 0;
}
 
node *getNode()
{
	return (node*)malloc(sizeof(node));
}
 
void init(list *pList)
{
	node **pHead = &(pList->head);
	node **pTail = &(pList->tail);
 
	*pHead = getNode();
	*pTail = getNode();
 
	(*pHead)->prev = NULL;
	(*pHead)->next = *pTail;
	(*pTail)->prev = *pHead;
	(*pTail)->next = NULL;
}
 
void add(list List, int position, char item)
{
	node *p;
 
	if (position <= 0)
		errorHandling();
 
	for (p=List.head; p!=NULL && 0<position; position--, p=p->next);
 
	if (p != NULL)
	{
		node *nw = getNode();
		nw->elem = item;
 
		nw->prev = p->prev;
		nw->next = p->prev->next;
		p->prev->next = nw;
		p->prev = nw;
	}
	else
	{
		errorHandling();
	}
}
 
void _delete(list List, int position)
{
	node *p;
 
	if (position <= 0)
		errorHandling();
 
	for (p=List.head; p!=List.tail && 0<position; position--, p=p->next);
 
	if (p != List.tail)
	{
		p->prev->next = p->next;
		p->next->prev = p->prev;
		free(p);
	}
	else
	{
		errorHandling();
	}	
}
 
char get_entry(list List, int position)
{
	node *p;
 
	if (position <= 0)
		errorHandling();
 
	for (p=List.head; p!=List.tail && 0<position; position--, p=p->next);
 
	if (p != List.tail)
	{
		return p->elem;
	}
	else
	{
		errorHandling();
		return 0;
	}
}
 
void print(list List)
{
	for (node *p=(List.head)->next; p!=List.tail; p=p->next)
		putchar(p->elem);
	puts("");
}
 
void cleanUp(list List)
{
	node *t;
 
	for (node *p=List.head; p!=NULL; p=t)
	{
		t = p->next;
		free(p);
	}
}
 
void errorHandling()
{
	puts("invalid position");
}