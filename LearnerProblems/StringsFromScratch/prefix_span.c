/*
Implement a function that measures the longest prefix 
made only of characters from an allowed set.
*/
#include <stddef.h>
int is_ok(char c, const char *allowed)
{
	int j = 0;
	while (allowed[j])
	{
		if (c == allowed[j])
			return (1);
		j++;
	}
	return (0);
}

size_t prefix_span(const char *text, const char *allowed)
{
	if (!text || !is_ok(*text, allowed))
		return (0);
	
	size_t count = 0;
	int i = -1;
	while (text[++i])
	{
		if (is_ok(text[i], allowed))
			count++;
	}
	return (count);
}