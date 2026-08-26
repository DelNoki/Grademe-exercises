#include <stddef.h>

void	*memmove(void *dst, const void *src, size_t n)
{
	size_t i = 0;
	unsigned char *d = (unsigned char *)dst;
	unsigned char *s = (unsigned char *)src;
	if (n == 0)
		return (dst);
	if (d > s)
	{
		while (i < n)
		{
			d[n - 1 - i] = s[n - 1 - i];
			i++;
		}
	}
	else
	{
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dst);
}