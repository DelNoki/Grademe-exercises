#include <unistd.h>

int maj_or_min (char c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	else if (c >= 'a' && c <= 'z')
		return (2);
	else
	return (0);
}

int	main(int argc, char **argv)
{
	int i = 0;
	int j;

	if (argc <= 2 || argc > 3)
		write(1, "wrong number of arguments\n", 26);
	else
	{
		j = 0;
		while (argv[2][i])
		{
			if (argv[2][i] == argv[1][j])
				j++;
			if (maj_or_min(argv[1][j]) == 1 && (argv[2][i] == argv[1][j] || argv[2][i] - 32 == argv[1][j]))
				j++;
			if (maj_or_min(argv[1][j]) == 2 && (argv[2][i] == argv[1][j] || argv[2][i] + 32 == argv[1][j]))
				j++;
			if (argv[1][j] == 0)
			{
				j = 0;
				while (argv[1][j])
				{
					write(1, &argv[1][j], 1);
					j++;
				}
				break;
			}
			i++;
		}
		write(1, "\n", 1);
	}
	return (0);
}