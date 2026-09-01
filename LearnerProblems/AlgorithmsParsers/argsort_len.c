/*
Implement a function that fills out with the indices of arr ordered 
by string length, shortest first, without touching arr itself.
*/

#include <stddef.h>

static size_t ft_len(const char *s)
{
	size_t i = 0;
	while (s[i])
		i++;
	return (i);
}

static int ft_strcmp(const char *s1, const char *s2)
{
	size_t i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/* true if index a must sort strictly before index b */
static int is_before(const char *const arr[], size_t a, size_t b)
{
	size_t la;
	size_t lb;
	int c;

	la = ft_len(arr[a]);
	lb = ft_len(arr[b]);
	if (la != lb)
		return (la < lb);
	c = ft_strcmp(arr[a], arr[b]);
	if (c != 0)
		return (c < 0);
	return (a < b); /* equal strings: lower index first (stable) */
}

void argsort_len(const char *const arr[], size_t n, size_t *out)
{
	size_t i;
	size_t j;
	size_t best;

	if (n == 0 || !arr)
		return;
	size_t used[n]; /* declared after the guard: n is >= 1 here */
	i = 0;
	while (i < n)
	{
		used[i] = 0;
		i++;
	}
	i = 0;
	while (i < n)
	{
		best = 0;
		while (used[best])
			best++;
		j = best + 1;
		while (j < n)
		{
			if (!used[j] && is_before(arr, j, best))
				best = j;
			j++;
		}
		out[i] = best;
		used[best] = 1;
		i++;
	}
}

/*
Autre version plus clean :

#include <stddef.h>

int _get_len(char *str)
{
	if(*str == '\0')
		return(0);
	return(1 + _get_len(str + 1));
}

int _str_cmp(char *s1, char *s2)
{
	int i;
	i = 0;
	while(s1[i] && s1[i] == s2[i])
		i++;
	return(s1[i] - s2[i]);
}
void _swapit(size_t i, size_t j, size_t *out)
{
	size_t swap;
	swap = out[i];
	out[i] = out[j];
	out[j] = swap;
}

void	argsort_len(char **arr, size_t n, size_t *out)
{
	size_t j;
	size_t i;

	if(n == 0)
		return;
	i = 0;
	while(i < n)
	{
		out[i] = i;
		i++;
	}
	i = 0;
	while(i < n)
	{
		j = i + 1;
		while(j < n)
		{
			if(_get_len(arr[out[i]]) > _get_len(arr[out[j]]))
				_swapit(i, j, out);

			else if(_get_len(arr[out[i]]) == _get_len(arr[out[j]]))
			{
				if(_str_cmp(arr[out[i]], arr[out[j]]) > 0)
					_swapit(i, j, out);
				else if(_str_cmp(arr[out[i]], arr[out[j]]) == 0
				&& out[i] > out[j])
					_swapit(i, j, out);
			}
			j++;
		}
		i++;
	}
	
}
*/