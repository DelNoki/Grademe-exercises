#include <stdlib.h>
#include <unistd.h>
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int h = atoi(argv[1]);
	if (h >= 1)
	{
		for (int i = 1; i <= h; i++) 
		{
			for (int j = 0 ; j < h + i - 1 ; j++) 
			{
				if (j >= 0 && j <= h - i - 1)
					write(1, " ", 1);
				else
					write(1, "#", 1);
			}
			write(1, "\n", 1);
		}
	}
	return (0);
}