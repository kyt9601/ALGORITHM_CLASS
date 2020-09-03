#include <stdio.h>

int rFindNum(int bottom, int top);

int main()
{
	int a, b, n;

	scanf("%d %d %d",&a,&b,&n); getchar();

	printf("%d\n",rFindNum(a,b));

	return 0;
}

int rFindNum(int bottom, int top)
{
	if (bottom != top)
		if (getchar() == 'Y')
			return rFindNum((bottom+top)/2 + 1, top);
		else
			return rFindNum(bottom, (bottom+top)/2);
	else
		return bottom;
}