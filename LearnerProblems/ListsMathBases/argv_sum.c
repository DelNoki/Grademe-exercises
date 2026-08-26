/*
Write a program that adds all integer arguments passed to it and prints the total.
*/

#include <unistd.h>

int ft_atoi(char *s)
{
	int i = 0;
	int sign = 0;
	int tot = 0;
	if (s[i] == '-')
	{
		sign++;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		tot = tot * 10 + s[i] - '0';
		i++;
	}
	if (sign == 1)
		tot *= -1;
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

int	main(int argc, char **argv)
{
	if (argc <= 1)
	{
		write(1, "0\n", 2);
		return (0);
	}
	int sum = 0;
	int i = 1;
	while (argv[i])
	{
		sum += ft_atoi(argv[i]);
		i++;
	}
	ft_putnbr(sum);
	write(1, "\n", 1);
	return (0);
}
