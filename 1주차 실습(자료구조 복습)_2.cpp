#include <stdio.h>
#include <stdlib.h>
 
typedef struct BinaryTree
{
	int id;
	struct BinaryTree *left;
	struct BinaryTree *right;
} node;
 
node *getNode();
void addNode(node **p);
void printPath(node *p, char *path);
void cleanUp(node *p);
 
int main()
{
	int n;
	char path[101];
	node *root;
 
	scanf("%d",&n);
	addNode(&root);
 
	scanf("%d",&n);
	for (int i=0; i<n; i++)
	{
		scanf("%s",path);
		printPath(root,path);
	}
 
	cleanUp(root);
 
	return 0;
}
 
node *getNode()
{
	return (node*)malloc(sizeof(node));
}
 
void addNode(node **p)
{
	int x, y, z;
 
	scanf("%d %d %d",&x,&y,&z);
	
	*p = getNode();
	(*p)->id = x;
	(*p)->left = NULL;
	(*p)->right = NULL;
 
	if (y) { addNode( &((*p)->left) ); }
	if (z) { addNode( &((*p)->right) ); }
}
 
void printPath(node *p, char *path)
{
	while (*path)
	{
		printf(" %d",p->id);
 
		if (*path == 'L')
			p=p->left;
		else
			p=p->right;
 
		path++;
	}
 
	printf(" %d\n",p->id);
}
 
void cleanUp(node *p)
{
	if (p->left != NULL) { cleanUp(p->left); }
	if (p->right != NULL) { cleanUp(p->right); }
	free(p);
}