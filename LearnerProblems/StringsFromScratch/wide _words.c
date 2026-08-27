/*
Write a program that normalizes one string argument 
by printing its words separated by exactly three spaces.
*/
#include <unistd.h>
 
int is_whitespace_or_printable(char c)
{
	if (c >= 33 && c <= 126)
		return 1;
	else if ((c >= 9 && c <= 13) || c == ' ')
		return 2;
	else
		return 0;
}
 
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int i = 0;
	int word = 0;   /* 1 while we are inside a word */
	int first = 1;  /* 1 until the first word has been printed */
 
	while (argv[1][i])
	{
		if (is_whitespace_or_printable(argv[1][i]) == 1)
		{
			if (word == 0)
			{
				if (!first)
					write(1, "   ", 3);
				word = 1;
				first = 0;
			}
			write(1, &argv[1][i], 1);
		}
		else if (is_whitespace_or_printable(argv[1][i]) == 2)
			word = 0;
		i++;
	}
	write(1, "\n", 1);
	return (0);
}