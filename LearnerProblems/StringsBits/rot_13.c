#include <unistd.h>

int	main(int argc, char **argv)
{
	char *alp = "abcdefghijklmnopqrstuvwxyz";
	char *Alp = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i = 0;
	int j;
	char *s;
	if (argc == 1 || argc > 2)
		write(1, "wrong number of arguments\n", 26);
	else
	{
		s = argv[1];
		while (s[i])
		{
			if (s[i] >= 'A' && s[i] <= 'Z')
			{
				j = 0;
				while (s[i] != Alp[j])
					j++;
				write(1, &Alp[(j + 13) % 26], 1);
			}
			else if (s[i] >= 'a' && s[i] <= 'z')
			{
				j = 0;
				while (s[i] != alp[j])
					j++;
				write(1, &alp[(j + 13) % 26], 1);
			}
			else
				write(1, &s[i], 1);
			i++;
		}
		write(1, "\n", 1);
	}
	return (0);
}