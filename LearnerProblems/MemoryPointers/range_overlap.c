/*
Write a function that returns 1 when two byte slices taken 
from the same array share at least one byte, and 0 when they do not.
*/
#include <stddef.h>
int	ranges_overlap(const void *a, size_t na, const void *b, size_t nb)
{
	size_t i = 0;
	size_t j;
	if (na == 0 || nb == 0)
		return (0);
	if (a == b)
		return (1);
	while (i < na)
	{
		j = 0;
		while (j < nb)
		{
			if (a + i == b + j)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}