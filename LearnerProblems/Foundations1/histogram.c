/*
Write a program that draws one bar of # per argument, 
where the bar for a value of n is n characters long and sits on its own line.
*/

#include <stdlib.h>
#include <unistd.h>
int	main(int argc, char **argv)
{
	int i;
	int n;

	if (argc >= 2)
	{
		i = 1;
		while (i < argc)
		{
			n = atoi(argv[i]);
			if (n > 0)
			{
				int n = atoi(argv[i]);
				while (n > 0)
				{
					write(1, "#", 1);
					n--;
				}
			}
			write(1, "\n", 1);
			i++;
		}
	}
	else
		write(1, "wrong number of arguments\n", 26);
	
	return (0);
}
