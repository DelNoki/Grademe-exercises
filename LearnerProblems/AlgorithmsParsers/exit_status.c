/*
Write a program that prints the sum of its integer arguments on 
standard output and reports every failure through its exit status, 
not through a message on standard output.
*/

#include <limits.h>
#include <unistd.h>

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

int valid_int(char *s)
{
	int i = 0;
	if (s[i] == 0)
		return (0);
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (s[i] == 0)
		return (0);
	while (s[i] >= '0' && s[i] <= '9')
		i++;
	if (s[i] != 0)
		return (0);
	return (1);
}

long ft_atoi(char *nb)
{
	long tot = 0;
	int sign = 0;
	int i = 0;
	if (nb[i] == '-' || nb[i] == '+')
	{
		if (nb[i] == '-')
			sign++;
		i++;
	}
	while (nb[i] >= '0' && nb[i] <= '9')
	{
		tot = tot * 10 + nb[i] - '0';
		i++;
	}
	if (sign == 1)
		tot = -tot;
	return (tot);
}

int	main(int argc, char **argv)
{
	if (argc <= 1)
		return (1);

	int i = 1;
	int sum = 0;

	while (i < argc)
	{
		if (valid_int(argv[i]) == 0)
			return (2);
		if (ft_atoi(argv[i]) < INT_MIN 
		|| ft_atoi(argv[i]) > INT_MAX 
		|| (long)sum + ft_atoi(argv[i]) < INT_MIN 
		|| (long)sum + ft_atoi(argv[i]) > INT_MAX)
			return (3);
		else
			sum += (int)ft_atoi(argv[i]);
		i++;
	}
	ft_putnbr(sum);
	write(1, "\n", 1);
	return (0);
}
