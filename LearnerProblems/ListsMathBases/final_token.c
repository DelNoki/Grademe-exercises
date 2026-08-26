#include <unistd.h>

int type_char(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	else if (c >= 33 && c <= 126)
		return (2);
	else
		return (0);
}



int nb_words(char *s)
{
	int i = 0;
	int word = 0;
	int count = 0;

	while (s[i])
	{
		if (word == 0 && type_char(s[i]) == 2)
		{
			word = 1;
			count++;
		}
		else if (word == 1 && type_char(s[i]) == 1)
			word = 0;
		i++;
	}
	return (count);
}

int	main(int argc, char **argv)
{
	int i = 0;
	int word = 0;
	int count = 0;
	int l;

	if (argc == 1 || argc > 2)
		write(1, "wrong number of arguments\n", 26);
	else
	{
		l = nb_words(argv[1]);
		while (argv[1][i])
		{
			if (word == 0 && type_char(argv[1][i]) == 2)
			{
				word = 1;
				count++;
				if (count == l)
				{
					while (argv[1][i] && type_char(argv[1][i]) == 2)
					{
						write(1, &argv[1][i], 1);
						i++;
					}
					break;
				}
			}
			else if (word == 1 && type_char(argv[1][i]) == 1)
				word = 0;
			
			i++;
		}
		write(1, "\n", 1);
	}
	return (0);
}