/*
Write a program that takes a string and prints it after 
folding each lowercase letter to its alphabetical opposite, then a newline.
*/

#include <unistd.h>

/* Uncomment to also handle uppercase */
void alp_fold(int *c)
{
	char *alp = "abcdefghijklmnopqrstuvwxyz";
	//char *Alp = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i = 0;
	if (*c >= 'a' && *c <= 'z')
	{
		while (*c != alp[i])
			i++;
		*c = (int)(alp[25 - i]);
	}
	/*
	else if (*c >= 'A' && *c <= 'Z')
	{
		while (*c != Alp[i])
			i++;
		*c = (int)(Alp[25 - i]);
	}
	*/
	else
		return;
}

int	main(int argc, char **argv)
{
	int c;
	int i;
	if (argc != 2)
	{
		write(1, "wrong number of arguments\n", 26);
		return (0);
	}
	i = 0;
	while (argv[1][i])
	{
		c = argv[1][i];
		alp_fold(&c);
		write(1, &c, 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}
