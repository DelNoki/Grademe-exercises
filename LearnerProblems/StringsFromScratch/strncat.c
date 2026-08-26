#include <stddef.h>

char	*strncat(char *dst, const char *src, size_t n)
{
	size_t i = 0;
	char *p = dst;
	if (n == 0)
		return (dst);
	while (*p)
		p++;
	while (i < n && src[i])
	{
		*p = src[i];
		p++;
		i++;
	}
	*p = 0;
	return (dst);
}