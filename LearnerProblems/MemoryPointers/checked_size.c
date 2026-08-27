/*
Write a function that says whether the product n * size fits in a size_t, 
writing that product through out when it fits and leaving out alone when it does not.
*/
#include <stddef.h>
#include <stdint.h>

int	checked_size(size_t n, size_t size, size_t *out)
{
	if (n != 0 && size > SIZE_MAX / n)
		return (0);
	*out = n * size;
	return (1);
}