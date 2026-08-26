int	sum_range(const int *first, const int *last)
{
	if (!first || !last)
	if (first == last)
		return (0);
	int tot = 0;
	int i = 0;
	while (first + i < last)
	{
		tot += *(first + i);
		i++;
	}
	return (tot);
}

/* 
Pour aller plus loin : 
contrôler l'overflow ; 
gérer les cas où last se situe avant first.
 */