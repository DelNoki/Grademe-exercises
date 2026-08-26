#include <unistd.h>

int nb_op_space_else(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
		return (2);
	else if (c == ' ')
		return (3);
	else
		return (0);
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
	int i = 0;
	int j = 0;
	int n;
	int stack[100];
	if (argc != 2)
	{
		write(1, "Error\n", 6);
		return (0);
	}
	while (argv[1][i])
	{
		if (nb_op_space_else(argv[1][i]) == 1)
		{
			n = 0;
			while (nb_op_space_else(argv[1][i]) == 1)
			{
				n = n * 10 + argv[1][i] - '0';
				i++;
			}
			stack[j] = n;
			j++;
		}
		if (nb_op_space_else(argv[1][i]) == 3)
			i++;
		if (nb_op_space_else(argv[1][i]) == 2)
		{
			if (j < 2)
			{
				write(1, "Error\n", 6);
				return (0);
			}
			if ((argv[1][i] == '/' || argv[1][i] == '%') && stack[j - 1] == 0)
			{
				write(1, "Error\n", 6);
				return (0);
			}
			switch (argv[1][i])
			{
				case '+' :
				{
					stack[j - 2] = stack[j - 2] + stack[j - 1];
					break;
				}
				case '-' :
				{
					stack[j - 2] = stack[j - 2] - stack[j - 1];
					break;
				}
				case '*' :
				{
					stack[j - 2] = stack[j - 2] * stack[j - 1];
					break;
				}
				case '/' :
				{
					stack[j - 2] = stack[j - 2] / stack[j - 1];
					break;
				}
				case '%' :
				{
					stack[j - 2] = stack[j - 2] % stack[j - 1];
					break;
				}
			}
			j--;
			i++;
		}
		if (nb_op_space_else(argv[1][i]) == 3)
			i++;
	}
	if (j != 1)
	{
		write(1, "Error\n", 6);
		return (0);
	}
	ft_putnbr(stack[0]);
	write(1, "\n", 1);
	return (0);
}