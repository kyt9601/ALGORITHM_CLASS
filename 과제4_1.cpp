// 교수님께서 올려주신 다른버전을 참고하여 작성했습니다.

#include <stdio.h>
#include <stdlib.h>

#define max(a,b) ((a)>(b) ? (a) : (b))
#define abs(x) ((x)>=0 ? (x) : -(x))

typedef struct BST
{
	int key;
	int height;
	struct BST *parent;
	struct BST *left;
	struct BST *right;
} node;

node *getNode();
int isRoot(node *p);
int isExternal(node *p);
int isAncestor(node *p, node *ancst);
node *sibling(node *p);
node *inOrderSucc(node *p);
void expandExternal(node *p);
node *reduceExternal(node **pRoot, node *p);
node *treeSearch(node *p, int key);
void insertItem(node **pRoot, int key);
int removeElement(node **pRoot, int key);
int findElement(node *root, int key);
void print(node *p);
void cleanUp(node *p);

void searchAndRepairAfterInsertion(node **pRoot, node *w);
void searchAndRepairAfterRemoval(node **pRoot, node *w);
int heightUpdateAndBalanceCheck(node *w);
node *restructure(node **pRoot, node *x);

int main()
{
	char op;
	int k;
	node *root = getNode();
	
	do
	{
		scanf("%c",&op);

		switch(op)
		{
		case 'i' :
			scanf("%d",&k);
			insertItem(&root,k);
			break;
		case 'd' :
			scanf("%d",&k);
			if (k == removeElement(&root,k))
				printf("%d\n",k);
			else
				puts("X");
			break;
		case 's' :
			scanf("%d",&k);
			if (k == findElement(root,k))
				printf("%d\n",k);
			else
				puts("X");
			break;
		case 'p' :
			print(root);
			puts("");
			break;
		case 'q' :
			break;
		}

		getchar();
	}
	while (op != 'q');

	cleanUp(root);
	
	return 0;
}

node *getNode()	// 노드 동적 할당과 초기화
{
	node *p = (node*)malloc(sizeof(node));

	p->parent = NULL;
	p->left = NULL;
	p->right = NULL;
	p->height = 0;

	return p;
}

int isRoot(node *p)	// 루트인지 판별
{
	return !( (int)(p->parent) );
}

int isExternal(node *p)	// 외부 노드인지 판별
{
	if (p->left==NULL && p->right==NULL)
		return 1;
	else
		return 0;
}

int isAncestor(node *p, node *ancst)	// ancst 가 p 의 조상인지 판별
{
	for ( ; p!=NULL && p!=ancst; p=p->parent);
	return (p == ancst);
}

node *sibling(node *p)	// p 의 형제를 반환
{
	if (p->parent->left == p)
		return p->parent->right;
	else
		return p->parent->left;
}

node *inOrderSucc(node *p)	// p 의 중위순회 계승자를 반환
{
	p = p->right;

	while (!isExternal(p))
		p = p->left;
	return p;
}

void expandExternal(node *p)	// p 를 내부 노드로 확장
{
	p->left = getNode();
	p->right = getNode();
	p->height = 1;
	p->left->parent = p;
	p->right->parent = p;
}

node *reduceExternal(node **pRoot, node *p)	// p 의 조부모와 형제를 부모-자식 관계로 만들고 p 와 부모를 할당 해제
{
	node *parnt = p->parent;
	node *gparnt = parnt->parent;
	node *sibl = sibling(p);

	if (isRoot(parnt))
		*pRoot = sibl;
	else
		if (gparnt->left == parnt)
			gparnt->left = sibl;
		else
			gparnt->right = sibl;
	sibl->parent = gparnt;
	
	free(p);
	free(parnt);

	return sibl;
}

node *treeSearch(node *p, int key)	// 해당 부트리 내에 key 가 존재하는지 확인
{
	if (isExternal(p))
		return p;
	else if (p->key == key)
		return p;
	else if (p->key > key)
		return treeSearch(p->left,key);
	else
		return treeSearch(p->right,key);
}

void insertItem(node **pRoot, int key)	// 트리에 key 삽입 (중복 허용 안함)
{
	node *w = treeSearch(*pRoot,key);

	if (isExternal(w))
	{
		w->key = key;
		expandExternal(w);
		searchAndRepairAfterInsertion(pRoot,w);
	}
}

int removeElement(node **pRoot, int key)	// 트리에서 key 삭제
{
	node *p = treeSearch(*pRoot,key);

	if (!isExternal(p))
	{
		int k = p->key;
		node *sibl;
		
		if (!isExternal(p->left) && !isExternal(p->right))
		{
			node *q = inOrderSucc(p);
			p->key = q->parent->key;
			sibl = reduceExternal(pRoot,q);
		}
		else if (isExternal(p->left))
		{
			sibl = reduceExternal(pRoot,p->left);
		}
		else
		{
			sibl = reduceExternal(pRoot,p->right);
		}
		
		searchAndRepairAfterRemoval(pRoot,sibl->parent);
		return k;	// key 가 존재하면 key 반환
	}
	else
	{
		return key - 1;	// key 가 존재하지 않으면 key - 1 반환
	}
}

void searchAndRepairAfterInsertion(node **pRoot, node *w)	// 삽입후 불균형 노드를 찾아 높이균형 속성을 복구
{
	node *z = w, *y, *x;

	while (heightUpdateAndBalanceCheck(z))
	{
		if (isRoot(z))
			return;
		z = z->parent;
	}

	y = (z->left->height > z->right->height ? z->left : z->right);
	
	if (y->left->height > y->right->height)
		x = y->left;
	else if (y->left->height < y->right->height)
		x = y->right;
	else
		if (isAncestor(w,y->left))
			x = y->left;
		else
			x = y->right;
	
	restructure(pRoot,x);
}

void searchAndRepairAfterRemoval(node **pRoot, node *w)	// 삭제후 불균형 노드를 찾아 높이균형 속성을 복구
{
	node *z = w, *y, *x;
	node *b;

	while (heightUpdateAndBalanceCheck(z))
	{
		if (isRoot(z))
			return;
		z = z->parent;
	}

	y = (z->left->height > z->right->height ? z->left : z->right);
	
	if (y->left->height > y->right->height)
		x = y->left;
	else if (y->left->height < y->right->height)
		x = y->right;
	else
		if (z->left == y)
			x = y->left;
		else
			x = y->right;
	
	b = restructure(pRoot,x);
	searchAndRepairAfterRemoval(pRoot,b);
}

int heightUpdateAndBalanceCheck(node *w)	// 높이 갱신과 균형 검사
{
	int balance;
	node *l, *r;

	if (w == NULL)
		return 1;

	l = w->left;
	r = w->right;

	w->height = max(l->height,r->height) + 1;

	balance = abs(l->height - r->height);
	return (balance < 2);
}

node *restructure(node **pRoot, node *x)	// 개조
{
	node *y = x->parent;
	node *z = y->parent;
	node *a, *b, *c;
	node *T0, *T1, *T2, *T3;

	if (z->key < y->key && y->key < x->key)
	{
		a = z;
		b = y;
		c = x;

		T0 = a->left;
		T1 = b->left;
		T2 = c->left;
		T3 = c->right;
	}
	else if (x->key < y->key && y->key < z->key)
	{
		a = x;
		b = y;
		c = z;

		T0 = a->left;
		T1 = a->right;
		T2 = b->right;
		T3 = c->right;
	}
	else if (z->key < x->key && x->key < y->key)
	{
		a = z;
		b = x;
		c = y;

		T0 = a->left;
		T1 = b->left;
		T2 = b->right;
		T3 = c->right;
	}
	else
	{
		a = y;
		b = x;
		c = z;

		T0 = a->left;
		T1 = b->left;
		T2 = b->right;
		T3 = c->right;
	}

	if (isRoot(z))
		*pRoot = b;
	else if (z->parent->left == z)
		z->parent->left = b;
	else
		z->parent->right = b;
	b->parent = z->parent;

	a->left = T0;
	T0->parent = a;
	a->right = T1;
	T1->parent = a;
	a->height = max(T0->height,T1->height) + 1;

	c->left = T2;
	T2->parent = c;
	c->right = T3;
	T3->parent = c;
	c->height = max(T2->height,T3->height) + 1;

	b->left = a;
	a->parent = b;
	b->right = c;
	c->parent = b;
	b->height = max(a->height,c->height) + 1;

	return b;
}

int findElement(node *root, int key)	// 트리에서 key 검색
{
	node *q = treeSearch(root,key);

	if (!isExternal(q))
		return q->key;	// key 가 존재하면 key 반환
	else
		return key - 1;	// key 가 존재하지 않으면 key - 1 반환
}

void print(node *p)	// 해당 부트리 내에 모든 내부 노드들을 전위순회 순서로 출력
{
	if (!isExternal(p))
	{
		printf(" %d",p->key);
		print(p->left);
		print(p->right);
	}
}

void cleanUp(node *p)	// 할당 받은 동적 메모리를 후위순회 순서로 해제
{
	if (p != NULL)
	{
		cleanUp(p->left);
		cleanUp(p->right);
		free(p);
	}
}