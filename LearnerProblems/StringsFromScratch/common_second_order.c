/*
Write a program that takes two strings and displays, without duplicates, 
the characters that appear in both strings, 
in the order they appear in the second string.
*/
#include <unistd.h>

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int i = 0;
	int j;
	int k;
	while (argv[2][i])
	{
		j = 0;
		while (argv[1][j] && argv[1][j] != argv[2][i])
			j++;
		if (argv[1][j] != 0)
		{
			k = 0;
			while (argv[2][k])
			{
				if (argv[2][k] == argv[1][j])
				{
					if (k < i)
						break;
					else if (k == i)
						write(1, &argv[1][j], 1);
				}
				k++;
			}
		}
		i++;
	}
	write(1, "\n", 1);
	return (0);
}