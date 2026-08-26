/*
Write a program that reads a single identifier 
from the command line and prints it in lowerCamelCase.
*/
#include <unistd.h>

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int i = 0;
	char c;
	while (argv[1][i])
	{
		if (argv[1][i] == '_')
		{
			i++;
			if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
				c = argv[1][i] - 32;
		}
		else
			c = argv[1][i];
		write(1, &c, 1);
		if (argv[1][i])
			i++;
		else
			break;
	}
	write(1, "\n", 1);
	return (0);
}