#include <unistd.h>

int ft_atoi(char *s)
{
	int i = 0;
	int tot = 0;
	while (s[i])
	{
		tot = tot * 10 + s[i] - '0';
		i++;
	}
	return (tot);
}

void print_hexa(unsigned int n)
{
	char c;
	unsigned int nbr = n;
	char *hexa = "0123456789abcdef";
	if (nbr > 15)
		print_hexa(n / 16);
	c = hexa[n % 16];
	write(1, &c, 1);
}

int	main(int argc, char **argv)
{
	unsigned int nb;

	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	nb = ft_atoi(argv[1]);
	print_hexa(nb);
	write(1, "\n", 1);
	return (0);
}