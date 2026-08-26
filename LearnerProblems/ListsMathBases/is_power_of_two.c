int is_power_of_2(unsigned int n)
{
	unsigned int nb = n;
	if (nb == 0 || (nb - (nb / 2) * 2) % 2 != 0)
		return (0);
	else if (nb == 1)
		return (1);
	else
		return (is_power_of_2(nb / 2));
}

/*
Deuxième approche :
int is_power_of_2(unsigned int n)
{
	if (!n)
		return (0);
	while (n != 0)
	{
		if (n % 2 && n != 1)
			return (0);
		n /= 2;
	}
	return (1);
}
*/