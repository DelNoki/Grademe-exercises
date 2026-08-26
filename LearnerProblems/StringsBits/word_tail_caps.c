/*
Write a program that transforms each argument 
so that the last alphabetic character of every word 
is uppercase and every other letter is lowercase.
*/

#include <unistd.h>

static int type_char(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else if (c >= 'A' && c <= 'Z')
		return (2);
	else if (c >= 'a' && c <= 'z')
		return (3);
	else
		return (0);
}

static void ft_putstr(const char *s)
{
	int i = 0;
	while (s[i])
		i++;
	write(1, s, i);
}

int	main(int argc, char **argv)
{
	if (argc <= 1)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int i = 1;
	int j;
	int word = 0;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
			j++;
		if (j == 1 && type_char(argv[i][0]) == 3)
		{
			char c = argv[i][0] - 32;
			write(1, &c, 1);
		}
        else
        {
		    j--;
		    while (j > 0)
		    {
			    if (word == 0 && type_char(argv[i][j]) != 0)
			    {
				    word = 1;
				    if (type_char(argv[i][j]) == 3)
					    argv[i][j] -= 32;
			    }
			    else if (word == 1 && type_char(argv[i][j]) == 2)
				    argv[i][j] += 32;
			    else if (word == 1 && type_char(argv[i][j]) == 0)
				    word = 0;
			    j--;
		    }
		    ft_putstr(argv[i]);
        }
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

/*
Alternative :
#include <unistd.h>
int is_print(char c)
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return 1;
	}
	return 0;
}
int	main(int argc, char **argv)
{
	if(argc >= 2)
	{
		int i = 1;
		int j = 0;
		
		while(i <= argc - 1)
		{
			j = 0;
			while(argv[i][j] != '\0')
			{
				if(argv[i][j + 1] == '\0' || (!is_print(argv[i][j + 1])) )
				{
					if(argv[i][j] >= 'a' && argv[i][j] <= 'z')
					{
						argv[i][j] = argv[i][j] - 32;
					}
				}
				else if(argv[i][j] >= 'A' && argv[i][j] <= 'Z')
				{
					argv[i][j] = argv[i][j] + 32;
				}
				write(1, &argv[i][j], 1);
				j++;
			}
			write(1, "\n", 1);
			i++;
		}

	}
	else
	{
		write(1, "wrong number of arguments\n", 26);
	}
	return (0);
}
*/