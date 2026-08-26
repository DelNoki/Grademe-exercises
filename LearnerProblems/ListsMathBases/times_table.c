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

int ft_atoi(char *nb)
{
	int i = 0;
	int tot = 0;
	while(nb[i])
	{
		tot = tot * 10 + nb[i] - '0';
		i++;
	}
	return (tot);
}

int	main(int argc, char **argv)
{
	int i = 1;
	char c;
	if (argc == 1 || argc > 2)
		write(1, "wrong number of arguments\n", 26);
	else
	{
		while (i <= 9)
		{
			c = i + '0';
			write(1, &c, 1);
			write(1, " x ", 3);
			ft_putnbr(ft_atoi(argv[1]));
			write(1, " = ", 3);
			ft_putnbr(i * ft_atoi(argv[1]));
			write(1, "\n", 1);
			i++;
		}
	}
	return (0);
}