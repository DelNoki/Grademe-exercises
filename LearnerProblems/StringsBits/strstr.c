#include <stddef.h>

char	*strstr(const char *haystack, const char *needle)
{
	int j;
	char *h = (char *)haystack;
	const char *n = needle;
	if (*n == 0)
		return (h);
	while (*h)
	{
		j = 0;
		while (*(h + j) == n[j] && n[j] && *(h + j))
			j++;
		if (n[j] == 0)
			return (h);
		h++;
	}
	return (NULL);
}

/*
#include <stdio.h>
int main(void)
{
	char *r = strstr("aaab", "aab");
	if (r == NULL)
		printf("(null)\n");
	else
		printf("%p  %s\n", r, r);
	r = strstr("ababc", "abc");
	if (r == NULL)
		printf("(null)\n");
	else
		printf("%p  %s\n", r, r);
	r = strstr("rebase", "rebase");
	if (r == NULL)
		printf("(null)\n");
	else
		printf("%p  %s\n", r, r);
	r = strstr("commit", "");
	if (r == NULL)
		printf("(null)\n");
	else
		printf("%p  %s\n", r, r);
	return 0;
}
*/