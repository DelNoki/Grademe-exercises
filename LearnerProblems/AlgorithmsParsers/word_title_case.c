#include <unistd.h>

int type_char(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	else if (c >= 'a' && c <= 'z')
		return (2);
	else if (c >= '0' && c <= '9')
		return (3);
	else
		return (0);
}

void print_title_case(char *s)
{
	int i = 0;
	int word = 0;
	char c;

	while (s[i])
	{
		if (word == 0 && type_char(s[i]) != 0)
		{
			word = 1;
			if (type_char(s[i]) == 2)
				c = s[i] - 32;
			else
				c = s[i];
		}
		else if (word == 1 && type_char(s[i]) == 1)
			c = s[i] + 32;
		else if (word == 1 && type_char(s[i]) == 0)
		{
			word = 0;
			c = s[i];
		}
		else
			c = s[i];
		write(1, &c, 1);
		i++;
	}
}

int	main(int argc, char **argv)
{
	int i = 1;

	if (argc < 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}

	while (argv[i] != (void *)0)
	{
		print_title_case(argv[i]);
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
