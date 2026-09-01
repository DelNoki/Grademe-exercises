/*
Write a program that receives a single string argument 
and prints it with every letter advanced by one position in the alphabet.
*/

#include <unistd.h>

void move_forward(int *c)
{
	char *alp = "abcdefghijklmnopqrstuvwxyz";
	char *Alp = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i = 0;
	if (*c >= 'a' && *c <= 'z')
	{
		while (*c != alp[i])
			i++;
		*c = alp[(i + 1) % 26];
	}
	else if (*c >= 'A' && *c <= 'Z')
	{
		while (*c != Alp[i])
			i++;
		*c = Alp[(i + 1) % 26];
	}
	else
		return;
}

int	main(int argc, char **argv)
{
	int i = 0;
	int c;
	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	while (argv[1][i])
	{
		c = argv[1][i];
		move_forward(&c);
		write(1, &c, 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}
