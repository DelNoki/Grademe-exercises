/*
Write a function that allocates and returns an array 
containing every integer from max down to min, inclusive.
*/
#include <stdlib.h>
int *range_desc(int min, int max)
{
	int l;
	if (max - min >= 0 && max - min <= 2147483647)
		l = max - min;
	else
		return (NULL);
	int *tab = (int *)malloc(sizeof(int) * (l + 1));
	if (!tab)
		return (NULL);
	int i = 0;
	while (i <= l)
	{
		*(tab + i) = max - i;
		i++;
	}
	return (tab);
}