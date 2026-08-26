/* Sum of all digits in a number */
int	digit_sum(int n)
{
	if (n == 0)
		return (0);
	long nb;
	if ((long)n < 0)
		nb = -(long)n;
	else
		nb = n;
	if (nb > 0)
		return (digit_sum(nb / 10) + (nb % 10));
	else
		return (nb);
}