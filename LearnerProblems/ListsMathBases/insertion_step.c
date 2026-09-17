/*
Sort a in place by insertion and hand the whole array to report after every insertion, 
so the caller can watch the sorted prefix grow one slot at a time.
*/

#include <stddef.h>

void	insertion_steps(int *a, size_t n, void (*report)(const int *, size_t))
{
	size_t i = 1;
	size_t j;
	int swap;
	if (n <= 1)
		return;
	while (i < n)
	{
		j = i;
		while (j > 0)
		{
			if (a[j] < a[j - 1])
			{
				swap = a[j];
				a[j] = a[j - 1];
				a[j - 1] = swap;
			}
			j--;
		}
		report(a, n);
		i++;
	}
}
