#include <stdlib.h>
#include <stddef.h>

int ft_strcat(char *src, char *dest, int index)
{
    int i = 0;
    while (src[i])
    {
        *(dest + index + i) = src[i];
        i++;
    }
    return (i);
}

int long_tot(char **tab, size_t n)
{
    size_t i = 0;
    int j;
    int count = 0;
    while (i < n)
    {
        j = 0;
        while (tab[i][j])
            j++;
        count += j;
        i++;
        if (i < n)
            count += 1;
    }
    return (count);
}

char	*str_join(char **parts, size_t n, char sep)
{
	size_t i = 0;
    int j = 0;
    int l = long_tot(parts, n);
    char *r = (char *)malloc(sizeof(char) * (l + 1));
    if (!r)
        return (NULL);
    while (i < n)
    {
        j += ft_strcat(parts[i], r, j);
        i++;
        if (i < n)
		{
			r[j] = sep;
			j++;
		}
    }
    r[j] = 0;
    return (r);
}
