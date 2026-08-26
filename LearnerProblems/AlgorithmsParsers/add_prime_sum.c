#include <unistd.h>

int is_nbr(char *s)
{
	int i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int ft_atoi(char *s)
{
	int i = 0;
	int tot = 0;
	if (!is_nbr(s))
		return (-1);
	while (s[i])
	{
		tot = tot * 10 + s[i] - '0';
		i++;
	}
	return (tot);
}

void ft_putnbr(int n)
{
	char c;

	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return;
	}
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	if (n > 9)
		ft_putnbr(n / 10);
	c = n % 10 + '0';
	write(1, &c, 1);
}

int is_prime(int n)
{
	int i = 2;
	int nb = n;
	while (i <= (nb / 2))
	{
		if (nb % i == 0)
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	int i = 2;
	int N;
	int sum = 0;
	if (argc != 2)
	{
		write(1, "0\n", 2);
		return (0);
	}
	N = ft_atoi(argv[1]);
	if (N == -1)
	{
		write(1, "0\n", 2);
		return (0);
	}
	while (i <= N)
	{
		if (is_prime(i))
			sum += i;
		i++;
	}
	ft_putnbr(sum);
	write(1, "\n", 1);
	return (0);
}
