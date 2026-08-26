#include <stdlib.h>
#include <unistd.h>

/* Write a line made of two characters : a for the ends, b for the middle */
void print_line(char a, char b, int l)
{
	int i = 0;
	while (i < l)
	{
		if (i == 0 || i == l - 1)
			write(1, &a, 1);
		else
			write(1, &b, 1);
		i++;
	}
	write(1, "\n", 1);
}

/* 
Print a box with the first and last rows made of '+' and '-' 
and the rest of the rows made of '|' and ' '.
*/
int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int w = atoi(argv[1]);
	int h = atoi(argv[2]);
	if (w > 0 && h > 0)
	{
		int i = 0;
		while (i < h)
		{
			if (i == 0 || i == h - 1)
				print_line('+', '-', w);
			else
				print_line('|', ' ', w);
			i++;
		}
	}
	return (0);
}
