#include <unistd.h>

int	main(int argc, char **argv)
{
	int i = 0;
	char *s;
	if (argc == 1)
		write(1, "wrong number of arguments\n", 26);
	else
	{
		s = argv[1];
		while (s[i])
		{
			write(1, &s[i], 1);
			i++;
		}
		write(1, "\n", 1);
	}
	return (0);
}