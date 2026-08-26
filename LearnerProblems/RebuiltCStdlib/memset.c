#include <stddef.h>

void	*memset(void *s, int c, size_t n)
{
	size_t i = 0;
	if (n == 0)
		return (s);
	while (i < n)
	{
		((unsigned char *)s)[i] = (unsigned char)c;
		i++;
	}
	return (s);
}