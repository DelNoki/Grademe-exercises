/*
Write a function that reports whether the first n values of a are 
in ascending order, where two equal neighbours still count as ordered.
*/

#include <stddef.h>

int	is_sorted(const int *a, size_t n)
{
	size_t i = 0;
	if (n > 1)
	{
		while(i < n - 1)
		{
			if (a[i] > a[i + 1])
				return (0);
			i++;
		}
	}
	return (1);
}