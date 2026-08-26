#include <stdlib.h>

int *int_range(int from, int to)
{
	int i;
	int l;
	int *r;
	if (to == from)
	{
		r = (int *)malloc(sizeof(int));
		if (!r)
			return (NULL);
		r[0] = from;
	}
	else if (to > from)
	{
		l = to - from;
		r = (int *)malloc(sizeof(int) * (l + 1));
		if (!r)
			return (NULL);
		i = 0;
		while (i <= l)
		{
			r[i] = from + i;
			i++;
		}
	}
	else
	{
		l = from - to;
		r = (int *)malloc(sizeof(int) * (l + 1));
		if (!r)
			return (NULL);
		i = 0;
		while (i <= l)
		{
			r[i] = from - i;
			i++;
		}
	}
	return (r);
}