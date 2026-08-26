/*
Implement strcspn, which scans a string from the beginning 
and returns the index of the first character that appears in a given reject set.
*/
#include <stddef.h>

size_t	strcspn(const char *s, const char *reject)
{
	int i = 0;
	int j;
	while (s[i])
	{
		j = 0;
		while (reject[j])
		{
			if (reject[j] == s[i])
				break;
			j++;
		}
		if (reject[j] == s[i])
			break;
		i++;
	}
	return (i);
}
