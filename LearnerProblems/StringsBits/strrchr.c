#include <stddef.h>

char	*strrchr(const char *s, int c)
{
	int i = 0;
	char *p = (char *)s;
	char *r = NULL;
	while (p[i])
	{
		if ((int)(p[i]) == c)
			r = &p[i];
		i++;
	}
	if ((int)(p[i]) == c)
		r = &p[i];
	return (r);
}