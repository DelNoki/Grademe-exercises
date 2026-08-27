/*
Implement a function that scans the first n bytes of a memory area 
for a given byte and returns a pointer to the first match, 
or NULL if it is absent.
*/
#include <stddef.h>

void	*memchr(const void *s, int c, size_t n)
{
	size_t i = 0;
	unsigned char *p = (unsigned char *)s;
	while(i < n)
	{
		if (*(p + i) == (unsigned char)c)
			return (p + i);
		i++;
	}
	return (NULL);
}