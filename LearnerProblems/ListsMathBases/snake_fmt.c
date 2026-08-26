#include <unistd.h>

int	main(int argc, char **argv)
{
	int i = 0;
	char c;
	if (argc == 1 || argc > 2)
		write(1, "wrong number of arguments\n", 26);
	else
	{
		while (argv[1][i])
		{
			if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
			{
				write(1, "_", 1);
				c = argv[1][i] + 32;
			}
			else
				c = argv[1][i];
			write(1, &c, 1);
			i++;
		}
		write(1, "\n", 1);
	}
	return (0);
}