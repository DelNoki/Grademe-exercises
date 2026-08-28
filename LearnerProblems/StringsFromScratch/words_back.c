#include <unistd.h>

int is_printable(char c)
{
	return (c >= 33 && c <= 126);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int i = 0;
	int j;
	while (argv[1][i])
		i++;
	while (i >= 0)
	{
		while (!is_printable(argv[1][i]) && i >= 0)
			i--;
		while (is_printable(argv[1][i]) && i >= 0)
			i--;
		j = i + 1;
		while (is_printable(argv[1][j]))
			write(1, &argv[1][j++], 1);
		if (i >= 0)
			write(1, " ", 1);
	}
	write(1, "\n", 1);
	return (0);
}