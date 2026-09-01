/*
Write a program that receives two strings and prints every unique character 
that appears in either one, without any duplicates. Output order is driven 
by the second string first, then any remaining characters from the first string.
*/

#include <unistd.h>

int already_encountered(int c, int i, char *s)
{
	int j = 0;
	while (j < i)
	{
		if (c == s[j])
			return (1);
		j++;
	}
	return (0);
}

int is_there(int c, char *s)
{
	int i = 0;
	while (s[i])
	{
		if (c == s[i])
			return (1);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	int i = 0;
	int c;
	while (argv[2][i])
	{
		c = argv[2][i];
		if (!already_encountered(c, i, argv[2]))
			write(1, &c, 1);
		i++;
	}
	i = 0;
	while(argv[1][i])
	{
		c = argv[1][i];
		if (!is_there(c, argv[2]) && !already_encountered(c, i, argv[1]))
			write(1, &c, 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}

/*
Alternative : on crée un tableau d'int qui contient des booléens pour chaque unsigned char.
Ensuite, quand on rencontre un caractère, on le met en "lu" dans le tableau.

#include <unistd.h>

int	main(int argc, char **argv)
{
	int i;
	int printed [256] = {0};

	if(argc != 3)
	{
		write(1,"wrong number of arguments\n",26);
		return 0;
	}

	i = 0;
	while(argv[2][i])
	{
		if(argv[2][i] && printed[(unsigned char)argv[2][i]] == 0)
		{
			write(1,&argv[2][i],1);
			printed[(unsigned char)argv[2][i]] = 1;
		}
		i++;
	}

	i = 0;
	while(argv[1][i])
	{
		if(argv[1][i] && printed[(unsigned char)argv[1][i]] == 0)
		{
			write(1,&argv[1][i],1);
			printed[(unsigned char)argv[1][i]] = 1;
		}
		i++;
	}
	write(1,"\n",1);
	return (0);
}
*/