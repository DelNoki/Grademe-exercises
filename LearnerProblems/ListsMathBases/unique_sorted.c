/*
Write a function that squeezes the duplicates out of an already 
sorted array in place and returns how many distinct values it kept.
*/

#include <stddef.h>

size_t unique_sorted(int *a, size_t n)
{
	if (n == 0)
		return (0);

	size_t write = 0;
	size_t read = 1;

	while (read < n)
	{
		if (a[read] != a[write])
		{
			write++;
			a[write] = a[read];
		}
		read++;
	}
	return (write + 1);
}