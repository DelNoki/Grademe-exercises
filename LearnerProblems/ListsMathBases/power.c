int	power(int base, int exp)
{
	if (exp < 0)
		return (-1);
	if (exp == 0)
		return (1);
	else
		return (power(base, exp - 1) * base);
}