#include <stddef.h>

void	*memcpy(void *dst, const void *src, size_t n)
{
	size_t i = 0;
	unsigned char *d = (unsigned char *)dst;
	unsigned char *s = (unsigned char *)src;
	if (n == 0)
		return (dst);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}