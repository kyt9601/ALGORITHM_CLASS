#include <stdio.h>
#include <stdlib.h>

typedef struct BST
{
    int key;
    struct BST *left;
    struct BST *right;
} node;

node *getNode();
void expandExternal(node *p);
int isExternal(node *p);
node *treeSearch(node *p, int key);
void insertItem(node *root, int key);
void print(node *p);
void cleanUp(node *p);

int main()
{
    int n;
    int key;
    node *root = getNode();

    scanf("%d",&n);

    for (int i=0; i<n; i++)
    {
	    scanf("%d",&key);
	    insertItem(root,key);
    }

    print(root);
    puts("");

    cleanUp(root);

    return 0;
}

node *getNode()
{
    node *p = (node*)malloc(sizeof(node));

    p->left = NULL;
    p->right = NULL;

    return p;
}

void expandExternal(node *p)
{
    p->left = getNode();
    p->right = getNode();
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
	    treeSearch(p->left,key);
    else
	    treeSearch(p->right,key);
}

void insertItem(node *root, int key)
{
    node *p = treeSearch(root,key);

    while (!isExternal(p))
	    p = treeSearch(p->right,key);

    p->key = key;
    expandExternal(p);
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