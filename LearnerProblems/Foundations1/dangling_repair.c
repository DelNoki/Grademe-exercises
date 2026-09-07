/*
Create a function such that every byte of the name#id string 
it returns is read from memory, then hand that one block to the caller.
*/

#include <stdlib.h>
#include <stddef.h>
static size_t	str_len(const char *s)
{
	size_t	n = 0;
	while (s[n] != '\0')
		n++;
	return (n);
}

char	*build_label(const char *name, int id)
{
	char			*digits;
	char			*out;
	unsigned int	mag;
	size_t			len;
	size_t			nd;
	size_t			i;

	if (name == NULL)
		return (NULL);
	len = str_len(name);
	mag = (unsigned int)id;
	if (id < 0)
		mag = 0u - mag; //u means that 0 is read as an unsigned char
	digits = malloc(10);
	if (digits == NULL)
		return (NULL);
	nd = 0;
	while (nd == 0 || mag != 0)
	{
		digits[nd++] = (char)('0' + mag % 10);
		mag /= 10;
	}
	out = malloc(len + nd + (size_t)(id < 0) + 2);
	if (out == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		out[i] = name[i];
		i++;
	}
	out[i++] = '#';
	if (id < 0)
		out[i++] = '-';
	while (nd > 0)
		out[i++] = digits[--nd];
	free(digits);
	out[i] = '\0';
	return (out);
}
