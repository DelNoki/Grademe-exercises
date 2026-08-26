/*
Write a program that receives a string, a search character, and a replacement character, 
then substitutes only the last matching character found in the string.
*/

#include <unistd.h>

static int ft_len(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	if (ft_len(argv[1]) != 0 && ft_len(argv[2]) == 1 && ft_len(argv[3]) == 1)
	{
		char *s = argv[1];
		char *to_r = (void *)0;

        /* check for last occurence of argv[2][0] */
		while (*s)
		{
			if (*s == argv[2][0])
				to_r = s;
			s++;
		}

        /* print argv[1] with the last occurence of argv[2][0] repaced if any */
        s = argv[1];
		while (*s)
		{
			if (s == to_r)
				write(1, &argv[3][0], 1);
			else
				write(1, s, 1);
			s++;
		}
	}
	write(1, "\n", 1);
	return (0);
}
