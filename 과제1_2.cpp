#include <stdio.h>
#include <stdlib.h>
 
typedef struct DoublyLinkedListWithHT	// 헤더와 트레일러 노드를 갖는 이중 연결 리스트
{
	char elem;
	struct DoublyLinkedListWithHT *prev;
	struct DoublyLinkedListWithHT *next;
} node;

typedef struct	// 리스트 정의
{
	node *head;
	node *tail;
} list;
 
node *getNode();
void init(list *pList);
node *add(list List, int position, char item);
char _delete(list List, int position);
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
		{		// A, D, G 연산의 함수들은 반환값을 이용하여 position 오류 처리
		case 'A' : scanf("%d %c",&pos,&item);
					if ( add(List,pos,item) == NULL ) { errorHandling(); } break;
		case 'D' : scanf("%d",&pos);
					if ( !(_delete(List,pos)) ) { errorHandling(); } break;
		case 'G' : scanf("%d",&pos);
					if ( (ch = get_entry(List,pos)) ) { putchar(ch); puts(""); }
					else { errorHandling(); } break;
		case 'P' : print(List); break;
		}
	}
 
	cleanUp(List);
 
	return 0;
}
 
node *getNode()	// 노드 동적 할당
{
	return (node*)malloc(sizeof(node));
}
 
void init(list *pList)	// 리스트 초기화
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
 
node *add(list List, int position, char item)	// 리스트에서 position 위치에 노드 삽입
{
	int i;
	node *p;

	// position 을 이용하여 리스트의 해당 위치 1칸 전으로 이동 (position 이 3일 경우 p 는 2 위치 노드의 주소값 저장)
	for (i=position, p=List.head; p!=List.tail && 1<i; i--, p=p->next);
 
	if (position<=0 || p==List.tail)
	{	// 유효하지 않은 위치
		return NULL;
	}
	else
	{
		node *q = getNode();
		q->elem = item;
 
		// p 의 다음 위치에 q 삽입
		q->next = p->next;
		q->prev = p->next->prev;
		p->next->prev = q;
		p->next = q;

		return q;
	}
}
 
char _delete(list List, int position)	// 리스트에서 position 위치에 해당하는 노드 삭제
{
	int i;
	node *p;

	// position 을 이용하여 리스트의 해당 위치로 이동 (위치는 p 에 저장)
	for (i=position, p=List.head; p!=List.tail && 0<i; i--, p=p->next);
 
	if (position<=0 || p==List.tail)
	{	// 유효하지 않은 위치
		return 0;
	}	
	else
	{
		char tmp = p->elem;

		// position 위치의 노드 삭제
		p->prev->next = p->next;
		p->next->prev = p->prev;
		free(p);

		return tmp;
	}
}
 
char get_entry(list List, int position)	// 리스트에서 position 위치에 해당하는 노드의 elem 값 출력
{
	int i;
	node *p;

	// position 을 이용하여 리스트의 해당 위치로 이동 (위치는 p 에 저장)
	for (i=position, p=List.head; p!=List.tail && 0<i; i--, p=p->next);
 
	if (position<=0 || p==List.tail)
		return 0;	// 유효하지 않은 위치
	else
		return p->elem;
}
 
void print(list List)	// 리스트를 순회하며 모든 노드의 elem 값 출력 (헤더/트레일러 노드 제외)
{
	for (node *p=(List.head)->next; p!=List.tail; p=p->next)
		putchar(p->elem);
	puts("");
}
 
void cleanUp(list List)	// 동적 할당된 메모리 해제
{
	node *t;
 #include <stdio.h>
#include <stdlib.h>
 
typedef struct BinaryTree	// 연결 이진 트리
{
	int id;
	struct BinaryTree *left;
	struct BinaryTree *right;
} node;
 
node *getNode();
void makeTree(node** pp);
void printPath(node *p, char *path);
void cleanUp(node *p);
 
int main()
{
	int n;
	char path[101];
	node *root = NULL;
 
	scanf("%d",&n);
	makeTree(&root);
 
	scanf("%d",&n);
	for (int i=0; i<n; i++)
	{
		scanf("%s",path);
		printPath(root,path);
	}
 
	cleanUp(root);
 
	return 0;
}
 
node *getNode()	// 노드 동적 할당
{
	return (node*)malloc(sizeof(node));
}

void makeTree(node** pp)	// 재귀를 이용한 트리 생성
{
	int x, y, z;
 
	scanf("%d %d %d",&x,&y,&z);	// 노드 번호 입력
	
	if (*pp == NULL)	// root 일때만 실행됨 (y/z 가 0일 경우 함수로 넘어오지 않음)
	{
		*pp = getNode();
		(*pp)->id = x;
	}

	// y 가 0 이 아니면 왼쪽 자식 생성
	if (y) { (*pp)->left = getNode(); (*pp)->left->id = y; }
	else { (*pp)->left = NULL; }	// --- (1)

	//  z 가 0 이 아니면 오른쪽 자식 생성
	if (z) { (*pp)->right = getNode(); (*pp)->right->id = z; }
	else { (*pp)->right = NULL; }

	if (y) { makeTree( &((*pp)->left) ); }	// --- (2)
	if (z) { makeTree( &((*pp)->right) ); }

	// (1) 과 (2) 는 반드시 둘 중 하나만 실행된다
}
 
void printPath(node *p, char *path)	// 경로 탐색중 방문하는 노드들의 번호 출력
{
	while (*path)	// 문자열의 마지막 '\0' 이 아닌동안 수행
	{
		printf(" %d",p->id);
 
		if (*path == 'L')
			p=p->left;
		else
			p=p->right;
 
		path++;
	}
 
	// 마지막 노드의 번호 출력
	printf(" %d\n",p->id);
}
 
void cleanUp(node *p)	// 후위순회 순서로 동적 할당된 메모리 해제
{
	if (p->left != NULL) { cleanUp(p->left); }
	if (p->right != NULL) { cleanUp(p->right); }
	free(p);
}
	for (node *p=List.head; p!=NULL; p=t)
	{
		t = p->next;
		free(p);
	}
}
 
void errorHandling()	// position 오류시 메세지 출력
{
	puts("invalid position");
}