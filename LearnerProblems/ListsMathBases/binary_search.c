/*
Implement a function that locates key in an increasing sequence you can only 
read one element at a time through the callback at, spending at most 24 reads.
*/

#include <stddef.h>

long	binary_search(size_t n, int key, int (*at)(size_t))
{
	long	lo;
	long	hi;
	long	mid;
	long	result;
	int		val;

	if (n == 0)
		return (-1);
	lo = 0;
	hi = (long)n - 1;
	result = -1;
	while (lo <= hi)
	{
		mid = lo + (hi - lo) / 2;
		val = at(mid);
		if (val < key)
			lo = mid + 1;
		else if (val > key)
			hi = mid - 1;
		else
		{
			result = mid;
			hi = mid - 1;
		}
	}
	return (result);
}