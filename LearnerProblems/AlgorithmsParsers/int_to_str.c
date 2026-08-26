#include <stdlib.h>

char *int_to_str(int n)
{
	unsigned int nb;
	int neg;
	int i;
	int l;
	char *r;

	neg = (n < 0);
	nb = neg ? -(unsigned int)n : (unsigned int)n;

	l = 1;
	while (nb / 10 >= 1 && ++l)
		nb /= 10;

	nb = neg ? -(unsigned int)n : (unsigned int)n;
	r = (char *)malloc(sizeof(char) * (l + neg + 1));
	if (!r)
		return (NULL);

	i = l + neg;
	r[i--] = 0;
	while (i >= neg)
	{
		r[i--] = (nb % 10) + '0';
		nb /= 10;
	}
	if (neg)
		r[0] = '-';
	return (r);
}

/* Première version : échec car erreur avec INT_MIN (pas d'équivalent positif...)
#include <stdlib.h>

char *if_minus(int size, char *s)
{
    int i = 0;
    char *r = (char *)malloc(sizeof(char) * (size + 2));
    if (!r)
        return (NULL);
    r[0] = '-';
    while (s[i])
    {
        r[i + 1] = s[i];
        i++;
    }
    r[i + 1] = 0;
    return (r);
}

char *int_to_str(int n)
{
	unsigned int nb;
	int i = 0;
	int l;
	char *r;

	if (n == 0)
	{
		r = (char *)malloc(sizeof(char) * 2);
		if (!r)
			return (NULL);
		r[0] = '0';
		r[1] = 0;
		return (r);
	}
    else if (n > 0)
    {
	    nb = n;
	    while (nb > 0)
	    {
		    nb = nb / 10;
		    i++;
	    }
	    l = i;
	    i = 0;
	    r = (char *)malloc(sizeof(char) * (l + 1));
	    if (!r)
		    return (NULL);
        nb = n;
	    while (nb > 0)
	    {
		    r[l - 1 - i] = nb % 10 + '0';
		    i++;
		    nb = (nb - (nb % 10)) / 10;
	    }
	    r[i] = 0;
	    return (r);
    }
    else
    {
        nb = -n;
        while (nb > 0)
	    {
		    nb = nb / 10;
		    i++;
	    }
	    l = i;
        return (if_minus(l, int_to_str(-n))); ICI PROBLEME
    }
}

#include <stdio.h>
int main(void)
{
	printf("%s\n", int_to_str(2048));
	return 0;
}
*/