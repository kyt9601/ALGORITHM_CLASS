#include <stdio.h>
#include <stdlib.h>

typedef struct BST
{
	int key;
	struct BST *parent;
	struct BST *left;
	struct BST *right;
} node;

node *getNode();
void insertItem(node *root, int key);
void removeElement(node *root, int key);
void findElement(node *root, int key);
node *treeSearch(node *p, int key);
void expandExternal(node *p);
void reduceExternal(node *p);
int isExternal(node *p);
node *sibling(node *p);
node *inOrderSucc(node *p);
void print(node *p);
void cleanUp(node *p);

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
			insertItem(root,k);
			break;
		case 'd' :
			scanf("%d",&k);
			removeElement(root,k);
			break;
		case 's' :
			scanf("%d",&k);
			findElement(root,k);
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

node *getNode()
{
	node *p = (node*)malloc(sizeof(node));

	p->parent = NULL;
	p->left = NULL;
	p->right = NULL;

	return p;
}

void expandExternal(node *p)
{
	p->left = getNode();
	p->right = getNode();
	p->left->parent = p;
	p->right->parent = p;
}

node *sibling(node *p)
{
	if (p->parent->left == p)
		return p->parent->right;
	else
		return p->parent->left;
}

void reduceExternal(node *p)
{
	node *parnt = p->parent;
	node *gparnt = parnt->parent;
	node *sibl = sibling(p);

	if (gparnt == NULL)
	{
		free(parnt->left);
		free(parnt->right);
		parnt->left = NULL;
		parnt->right = NULL;
	}
	else
	{
		if (gparnt->left == parnt)
			gparnt->left = sibl;
		else
			gparnt->right = sibl;
		sibl->parent = gparnt;

		free(parnt);
		free(p);
	}
}

int isExternal(node *p)
{
	if (p->left==NULL && p->right==NULL)
		return 1;
	else
		return 0;
}

node *treeSearch(node *p, int key)
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

void insertItem(node *root, int key)
{
	node *q = treeSearch(root,key);

	if (isExternal(q))
	{
		q->key = key;
		expandExternal(q);
	}
}

node *inOrderSucc(node *p)
{
	p = p->right;

	while (!isExternal(p))
		p = p->left;
	return p;
}

void removeElement(node *root, int key)
{
	node *p = treeSearch(root,key);

	if (!isExternal(p))
	{
		int k = p->key;
		
		if (!isExternal(p->left) && !isExternal(p->right))
		{
			node *q = inOrderSucc(p);
			p->key = q->parent->key;
			reduceExternal(q);
		}
		else if (isExternal(p->left))
		{
			reduceExternal(p->left);
		}
		else
		{
			reduceExternal(p->right);
		}
		
		printf("%d\n",k);
	}
	else
	{
		puts("X");
	}
}

void findElement(node *root, int key)
{
	node *q = treeSearch(root,key);

	if (!isExternal(q))
		printf("%d\n",q->key);
	else
		puts("X");
}

void print(node *p)
{
	if (!isExternal(p))
	{
		printf(" %d",p->key);
		print(p->left);
		print(p->right);
	}
}

void cleanUp(node *p)
{
	if (p != NULL)
	{
		cleanUp(p->left);
		cleanUp(p->right);
		free(p);
	}
}