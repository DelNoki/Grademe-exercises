#include <stddef.h>

char	*strchr(const char *s, int c)
{
	int i = 0;
	char *p = (char *)s;
	while (p[i])
	{
		if ((int)(p[i]) == c)
			return (&p[i]);
		i++;
	}
	if ((int)(p[i]) == c)
		return (&p[i]);
	return (NULL);
}